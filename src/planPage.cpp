#include <glibmm/main.h>
#include <glibmm/markup.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/eventcontrollerfocus.h>
#include <gtkmm/eventcontrollerkey.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/gesturedrag.h>
#include <gtkmm/scrolledwindow.h>

#include <memory>

#include <gdk/gdkkeysyms.h>

#include "app.hpp"
#include "functionality/plans/plans.hpp"
#include "functionality/lists/lists.hpp"
#include "functionality/utils.hpp"

Gtk::Box* planPage(Gtk::Stack* stack, json& appData, const Glib::ustring& planName) {
    auto planPage = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    auto header = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
    header->add_css_class("appHeader");

    // Go back button
    auto backBtn = Gtk::make_managed<Gtk::Button>();
    auto backIcon = Gtk::make_managed<Gtk::Image>();
    backIcon->set_from_icon_name("go-previous-symbolic");
    backBtn->set_child(*backIcon);
    backBtn->signal_clicked().connect([stack]() { stack->set_visible_child("list"); });

    // Plan name
    auto planNameLabel = Gtk::make_managed<Gtk::Label>(planName);
    planNameLabel->add_css_class("headerText");
    planNameLabel->set_hexpand(true);

    header->append(*backBtn);
    header->append(*planNameLabel);

    // Scrollable container for lists (horizontal)
    auto listsBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 12);
    listsBox->set_margin_start(12);
    listsBox->set_margin_end(12);
    listsBox->set_margin_top(8);
    listsBox->set_margin_bottom(8);

    auto scroll = Gtk::make_managed<Gtk::ScrolledWindow>();
    scroll->set_child(*listsBox);
    scroll->set_vexpand(true);
    scroll->set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);

    // Drag to scroll
    auto drag = Gtk::GestureDrag::create();
    auto startH = std::make_shared<double>(0.0);
    auto startV = std::make_shared<double>(0.0);
    drag->signal_drag_begin().connect([scroll, startH, startV](double, double) {
        *startH = scroll->get_hadjustment()->get_value();
        *startV = scroll->get_vadjustment()->get_value();
    });
    drag->signal_drag_update().connect([scroll, startH, startV](double dx, double dy) {
        scroll->get_hadjustment()->set_value(*startH - dx);
        scroll->get_vadjustment()->set_value(*startV - dy);
    });
    scroll->add_controller(drag);

    // Refresh function to rebuild all lists from JSON
    auto refreshLists = [listsBox, &appData, planName]() {
        while (auto child = listsBox->get_first_child())
            listsBox->remove(*child);

        std::string name = planName;
        for (auto& plan: appData["plans"]) {
            if (!plan.contains(name)) continue;
            auto& planData = plan[name];

            for (auto it = planData.begin(); it != planData.end(); ++it) {
                std::string listName = it.key();
                auto& list = it.value();

                auto listSection = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL, 4);
                listSection->set_size_request(300, -1);
                listSection->set_valign(Gtk::Align::START);

                // List header: name + delete button
                auto listHeader = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 6);
                auto listLabel = Gtk::make_managed<Gtk::Label>(listName);
                listLabel->set_markup("<b>" + Glib::Markup::escape_text(listName) + "</b>");
                listLabel->set_hexpand(true);
                listLabel->set_halign(Gtk::Align::START);
                listHeader->append(*listLabel);

                auto deleteListBtn = Gtk::make_managed<Gtk::Button>();
                deleteListBtn->set_icon_name("edit-delete-symbolic");
                deleteListBtn->signal_clicked().connect([&appData, name, listName]() {
                    deleteListFromPlanJSON(appData, name, listName);
                    saveJSON(appData);
                    App::get()->openPlan(name);
                });
                listHeader->append(*deleteListBtn);
                listSection->append(*listHeader);

                // Editable entry rows
                for (auto& entry: list["entries"]) {
                    int entryId = entry["id"].get<int>();
                    std::string entryVal = entry["value"].get<std::string>();

                    auto entryRow = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 6);
                    auto entryEdit = Gtk::make_managed<Gtk::Entry>();
                    entryEdit->set_text(entryVal);
                    entryEdit->set_hexpand(true);
                    entryRow->append(*entryEdit);

                    // Save on Enter or focus-out
                    entryEdit->signal_activate().connect([entryEdit, &appData, name, listName, entryId]() {
                        auto newVal = std::string(entryEdit->get_text());
                        if (newVal.empty())
                            deleteEntryFromListJSON(appData, name, listName, entryId);
                        else
                            editEntryInListJSON(appData, name, listName, entryId, newVal);

                        saveJSON(appData);
                    });

                    auto focusCtrl = Gtk::EventControllerFocus::create();
                    focusCtrl->signal_leave().connect([entryEdit, &appData, name, listName, entryId]() {
                        auto newVal = std::string(entryEdit->get_text());
                        if (newVal.empty()) {
                            deleteEntryFromListJSON(appData, name, listName, entryId);
                            saveJSON(appData);
                            App::get()->openPlan(name);
                        } else {
                            editEntryInListJSON(appData, name, listName, entryId, newVal);
                            saveJSON(appData);
                        };
                    });
                    entryEdit->add_controller(focusCtrl);

                    auto deleteEntryBtn = Gtk::make_managed<Gtk::Button>();
                    deleteEntryBtn->set_icon_name("edit-delete-symbolic");
                    deleteEntryBtn->signal_clicked().connect([&appData, name, listName, entryId]() {
                        deleteEntryFromListJSON(appData, name, listName, entryId);
                        saveJSON(appData);
                        App::get()->openPlan(name);
                    });
                    entryRow->append(*deleteEntryBtn);
                    listSection->append(*entryRow);
                }

                // Add entry row
                auto addEntryRow = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 6);
                auto entryInput = Gtk::make_managed<Gtk::Entry>();
                entryInput->set_placeholder_text("New entry");
                entryInput->set_hexpand(true);
                addEntryRow->append(*entryInput);

                auto addEntryBtn = Gtk::make_managed<Gtk::Button>();
                addEntryBtn->set_icon_name("list-add-symbolic");
                auto addEntry = [entryInput, &appData, name, listName]() {
                    auto val = entryInput->get_text();
                    if (!val.empty()) {
                        addEntryToListJSON(appData, name, listName, val);
                        saveJSON(appData);
                        App::get()->openPlan(name, listName);
                    }
                };
                entryInput->signal_activate().connect(addEntry);
                addEntryBtn->signal_clicked().connect(addEntry);
                addEntryRow->append(*addEntryBtn);
                listSection->append(*addEntryRow);

                // Track for auto-focus
                if (listName == App::get()->focusedList) {
                    auto* inputPtr = entryInput;
                    Glib::signal_idle().connect_once([inputPtr]() {
                        inputPtr->grab_focus();
                    });
                };

                listsBox->append(*listSection);
            };
            break;
        }
    };

    // Add list button (in header)
    auto addListBtn = makeListButton(planName, [planName]() {
        App::get()->openPlan(planName);
    });
    header->append(*addListBtn);

    planPage->append(*header);
    planPage->append(*scroll);

    // Escape and plan page deletion
    auto key = Gtk::EventControllerKey::create();
    planPage->add_controller(key);
    key->signal_key_pressed().connect([stack, planName, &appData](guint keyval, guint keycode, Gdk::ModifierType state) {
        if (keyval == GDK_KEY_Escape) {
            stack->set_visible_child("list"); // Go back
            return true;
        };

        // Delete current page
        bool ctrl = static_cast<bool>(state & Gdk::ModifierType::CONTROL_MASK); // is holding control
        if (ctrl && keyval == GDK_KEY_d) {
            stack->set_visible_child("list"); // Go back
            deletePlanFromJSON(appData, planName);
            saveJSON(appData);
            App::get()->listPage->refresh();
            return true;
        };

        return false;
    }, false);

    refreshLists(); // Show

    return planPage;
};
