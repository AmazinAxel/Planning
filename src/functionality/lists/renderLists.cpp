#include <glibmm/main.h>
#include <glibmm/markup.h>
#include <gtkmm/box.h>
#include <gtkmm/entry.h>
#include <gtkmm/eventcontrollerfocus.h>
#include <gtkmm/eventcontrollerkey.h>
#include <gtkmm/label.h>
#include <memory>
#include <gdk/gdkkeysyms.h>

#include "../../app.hpp"
#include "lists.hpp"
#include "../utils.hpp"

void renderLists(Gtk::Box* listsBox, json& appData, const std::string& planName) {
    while (auto child = listsBox->get_first_child())
        listsBox->remove(*child); // Removes all the old lists TODO replace this with something more modular so it doesnt revert positioning

    for (auto& plan: appData["plans"]) { // Loop all plans
        auto& planData = plan[planName];

        // Loop all lists in plan
        for (auto it = planData.begin(); it != planData.end(); ++it) {
            std::string listName = it.key();
            auto& list = it.value();

            auto listSection = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL, 4);
            listSection->set_size_request(300, -1);
            listSection->set_spacing(0); // All entries link so it feels like a big text box
            listSection->add_css_class("listSection");

            auto listTitle = Gtk::make_managed<Gtk::Label>(listName);
            listTitle->add_css_class("listHeader");
            listSection->append(*listTitle);

            // Loop all entries in list
            for (auto& entry: list["entries"]) {
                int entryID = entry["id"].get<int>();
                std::string entryData = entry["value"].get<std::string>();

                auto entryRow = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 6);
                auto entryEdit = Gtk::make_managed<Gtk::Entry>();
                entryEdit->set_text(entryData);
                entryEdit->set_hexpand(true);
                entryRow->append(*entryEdit);
                auto handled = std::make_shared<bool>(false); // fix bug

                // Enter: inserts a new entry below
                entryEdit->signal_activate().connect([entryEdit, &appData, planName, listName, entryID, handled]() {
                    auto newVal = std::string(entryEdit->get_text());
                    *handled = true;
                    Glib::signal_idle().connect_once([&appData, planName, listName, entryID, newVal]() {
                        if (newVal.empty()) { // Don't make new entry if the current one is empty
                            deleteEntryFromListJSON(appData, planName, listName, entryID);
                            saveJSON(appData);
                            App::get()->openPlan(planName, listName);
                            return;
                        };
                        editEntryInListJSON(appData, planName, listName, entryID, newVal);
                        int newID = addEntryToListJSON(appData, planName, listName, entryID);
                        saveJSON(appData);
                        App::get()->focusedEntryID = newID;
                        App::get()->openPlan(planName, listName);
                    });
                });

                // Backspace: delete and focus the previous entry or delete list
                auto keyCtrl = Gtk::EventControllerKey::create();
                keyCtrl->set_propagation_phase(Gtk::PropagationPhase::CAPTURE); // fix bug
                keyCtrl->signal_key_pressed().connect([entryEdit, &appData, planName, listName, entryID, handled](guint keyval, guint, Gdk::ModifierType) -> bool {
                    if (keyval == GDK_KEY_BackSpace && entryEdit->get_text().empty()) {
                        int prevID = -1;
                        for (auto& p: appData["plans"]) {
                            if (!p.contains(planName)) continue;
                            for (auto& e : p[planName][listName]["entries"]) {
                                int id = e["id"].get<int>();
                                if (id == entryID) break;
                                prevID = id;
                            };
                            break;
                        };
                        *handled = true;

                        // Delete list
                        if (prevID == -1) {
                            deleteListFromPlanJSON(appData, planName, listName);
                            saveJSON(appData);
                            Glib::signal_idle().connect_once([planName]() {
                                App::get()->openPlan(planName);
                            });
                        } else { // Go to prev entry
                            deleteEntryFromListJSON(appData, planName, listName, entryID);
                            saveJSON(appData);
                            App::get()->focusedEntryID = prevID;
                            Glib::signal_idle().connect_once([planName, listName]() {
                                App::get()->openPlan(planName, listName);
                            });
                        };
                        return true;
                    };
                    return false;
                }, false);
                entryEdit->add_controller(keyCtrl);

                // Focus
                if (listName == App::get()->focusedList && entryID == App::get()->focusedEntryID) {
                    App::get()->focusedEntryID = -1;
                    auto* inputPtr = entryEdit;
                    Glib::signal_idle().connect_once([inputPtr]() { inputPtr->grab_focus(); });
                };

                listSection->append(*entryRow);
            };

            listsBox->append(*listSection);
        };
        break;
    };
};
