#include <glibmm/main.h> // holy moly thats a lot of imports
#include <glibmm/markup.h>
#include <gtkmm/box.h>
#include <gtkmm/dragsource.h>
#include <gtkmm/droptarget.h>
#include <gtkmm/entry.h>
#include <gtkmm/eventcontrollerfocus.h>
#include <gtkmm/eventcontrollerkey.h>
#include <gtkmm/label.h>
#include <gdkmm/contentprovider.h>
#include <gdk/gdkcontentprovider.h>
#include <memory>
#include <vector>
#include <gdk/gdkkeysyms.h>
#include <gtkmm/widgetpaintable.h>
#include <gtkmm/dragsource.h>

#include "../../app.hpp"
#include "lists.hpp"
#include "../utils.hpp"

void renderLists(Gtk::Box* listsBox, json& appData, const std::string& planName) {
    while (auto child = listsBox->get_first_child())
        listsBox->remove(*child); // Removes all the old lists TODO replace this with something more modular so it doesnt revert positioning

    for (auto& plan: appData["plans"]) { // Loop all plans
        if (!plan.contains(planName)) continue;
        auto& planData = plan[planName];

        // Loop all lists in plan
        for (auto it = planData.begin(); it != planData.end(); ++it) {
            std::string listName = it.key();
            auto& list = it.value();

            auto listSection = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
            listSection->set_size_request(300, -1);
            listSection->set_spacing(0); // All entries link so it feels like a big text box
            listSection->add_css_class("listSection");

            auto listTitle = Gtk::make_managed<Gtk::Label>(listName);
            listTitle->add_css_class("listHeader");
            listSection->append(*listTitle);

            // Drop indicator state for this list
            auto activeIndicator = std::make_shared<Gtk::Box*>(nullptr);
            auto showIndicator = [activeIndicator](Gtk::Box* sep) {
                if (*activeIndicator) (*activeIndicator)->set_visible(false);
                *activeIndicator = sep;
                if (sep) sep->set_visible(true);
            };

            struct EntryInfo {
                int id;
                int prevID;
                Gtk::Box* row;
                Gtk::Box* sepAbove;
                Gtk::Entry* edit;
            };
            std::vector<EntryInfo> infos;
            int prevID = -1;

            // Loop all entries in list
            for (auto& entry: list["entries"]) {
                int entryID = entry["id"].get<int>();
                std::string entryData = entry["value"].get<std::string>();
                bool isIndented = entry.value("isIndented", false);

                // Drop indicator above this entry
                auto sep = Gtk::make_managed<Gtk::Box>();
                sep->add_css_class("dropIndicator");
                sep->set_visible(false);
                listSection->append(*sep);

                auto entryRow = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 0);
                auto entryEdit = Gtk::make_managed<Gtk::Entry>();
                if (isIndented)
                    entryEdit->add_css_class("indented");
                entryEdit->set_text(entryData);
                entryEdit->set_hexpand(true);
                entryRow->append(*entryEdit);
                auto handled = std::make_shared<bool>(false); // fix bug

                auto escapeConfirm = std::make_shared<bool>(false);

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
                keyCtrl->signal_key_pressed().connect([entryEdit, &appData, planName, listName, entryID, handled, escapeConfirm](guint keyval, guint, Gdk::ModifierType state) -> bool {
                    if (keyval == GDK_KEY_Escape) {
                        if (*escapeConfirm) // Go back
                            App::get()->stack->set_visible_child("list");
                        else // Select text first time
                            entryEdit->select_region(0, -1);
                        *escapeConfirm = true;
                        return true;
                    };
                    *escapeConfirm = false;

                    // Indent/de-indent
                    if (keyval == GDK_KEY_Tab || keyval == GDK_KEY_ISO_Left_Tab) {
                        bool shiftHeld = static_cast<int>(state & Gdk::ModifierType::SHIFT_MASK) != 0;
                        auto newVal = std::string(entryEdit->get_text());
                        editEntryInListJSON(appData, planName, listName, entryID, newVal);
                        setEntryIndentInListJSON(appData, planName, listName, entryID, !shiftHeld);
                        saveJSON(appData);
                        App::get()->focusedEntryID = entryID;
                        *handled = true;
                        Glib::signal_idle().connect_once([&appData, planName, listName]() {
                            App::get()->openPlan(planName, listName);
                        });
                        return true;
                    };
                    if (keyval == GDK_KEY_BackSpace && entryEdit->get_text().empty()) {
                        int prevID = -1, nextID = -1;
                        for (auto& p: appData["plans"]) {
                            if (!p.contains(planName)) continue;
                            bool found = false;
                            for (auto& e: p[planName][listName]["entries"]) {
                                int id = e["id"].get<int>();
                                if (found) { nextID = id; break; }

                                if (id == entryID)
                                    found = true;
                                else prevID = id;
                            };
                            break;
                        };
                        *handled = true;

                        if (prevID == -1 && nextID == -1) {
                            deleteListFromPlanJSON(appData, planName, listName);
                            saveJSON(appData);
                            Glib::signal_idle().connect_once([planName]() {
                                App::get()->openPlan(planName);
                            });
                        } else { // Prev or next entry
                            deleteEntryFromListJSON(appData, planName, listName, entryID);
                            saveJSON(appData);
                            App::get()->focusedEntryID = (prevID != -1) ? prevID : nextID;
                            Glib::signal_idle().connect_once([planName, listName]() {
                                App::get()->openPlan(planName, listName);
                            });
                        };
                        return true;
                    };
                    return false;
                }, false);
                entryEdit->add_controller(keyCtrl);

                // Save text on focus leave so re-renders wont break stuff
                auto focusCtrl = Gtk::EventControllerFocus::create();
                focusCtrl->signal_leave().connect([entryEdit, &appData, planName, listName, entryID, handled, escapeConfirm]() {
                    *escapeConfirm = false;
                    if (*handled) return;
                    editEntryInListJSON(appData, planName, listName, entryID, std::string(entryEdit->get_text()));
                    saveJSON(appData);
                });
                entryEdit->add_controller(focusCtrl);

                // Focus
                if (listName == App::get()->focusedList && entryID == App::get()->focusedEntryID) {
                    App::get()->focusedEntryID = -1;
                    auto* inputPtr = entryEdit;
                    Glib::signal_idle().connect_once([inputPtr]() { inputPtr->grab_focus(); });
                };

                listSection->append(*entryRow);
                infos.push_back({ entryID, prevID, entryRow, sep, entryEdit });
                prevID = entryID;
            };

            // End of list drop line
            auto sepEnd = Gtk::make_managed<Gtk::Box>();
            sepEnd->add_css_class("dropIndicator");
            sepEnd->set_visible(false);
            listSection->append(*sepEnd);

            // Drag sources and drop targets
            for (size_t i = 0; i < infos.size(); ++i) {
                EntryInfo info = infos[i];
                Gtk::Box* sepAfter = (i + 1 < infos.size()) ? infos[i + 1].sepAbove : sepEnd;

                // Drag source on all rows
                auto dragSrc = Gtk::DragSource::create();
                dragSrc->set_propagation_phase(Gtk::PropagationPhase::CAPTURE);
                dragSrc->set_actions(Gdk::DragAction::MOVE);
                auto paintable = Gtk::WidgetPaintable::create(*info.row); // Drag icon
                dragSrc->set_icon(paintable, 0, 0);
                dragSrc->set_propagation_phase(Gtk::PropagationPhase::CAPTURE);
                dragSrc->set_actions(Gdk::DragAction::MOVE);
                dragSrc->signal_prepare().connect([planName, listName, info](double, double) -> Glib::RefPtr<Gdk::ContentProvider> {
                    App::get()->dragPlanName = planName;
                    App::get()->dragListName = listName;
                    App::get()->dragEntryID = info.id;
                    GValue gval = G_VALUE_INIT;
                    g_value_init(&gval, G_TYPE_INT);
                    g_value_set_int(&gval, info.id);
                    auto provider = Glib::wrap(gdk_content_provider_new_for_value(&gval), true);
                    g_value_unset(&gval);
                    return provider;
                }, false);
                dragSrc->signal_drag_end().connect([info, activeIndicator](const Glib::RefPtr<Gdk::Drag>&, bool) {
                    info.row->set_opacity(1.0);
                    if (*activeIndicator) { (*activeIndicator)->set_visible(false); *activeIndicator = nullptr; }
                });
                info.edit->add_controller(dragSrc);

                // Drop target on row
                auto dropTarget = Gtk::DropTarget::create(G_TYPE_INT, Gdk::DragAction::MOVE);
                auto onMotion = [info, sepAfter, showIndicator](double, double y) -> Gdk::DragAction {
                    showIndicator(y < info.row->get_height() / 2.0 ? info.sepAbove : sepAfter);
                    return Gdk::DragAction::MOVE;
                };
                dropTarget->signal_enter().connect(onMotion, false);
                dropTarget->signal_motion().connect(onMotion, false);
                dropTarget->signal_leave().connect([showIndicator]() { showIndicator(nullptr); });
                dropTarget->signal_drop().connect([planName, listName, info, sepAfter, showIndicator, &appData](const Glib::ValueBase&, double, double y) -> bool {
                    showIndicator(nullptr);
                    info.row->set_opacity(1.0);
                    int srcID = App::get()->dragEntryID;
                    if (srcID == info.id) return false;
                    int insertAfterID = (y < info.row->get_height() / 2.0) ? info.prevID : info.id;
                    Glib::signal_idle().connect_once([planName, listName, srcID, insertAfterID, &appData]() {
                        moveEntryJSON(appData, App::get()->dragPlanName, App::get()->dragListName, srcID,
                                      planName, listName, insertAfterID);
                        saveJSON(appData);
                        App::get()->openPlan(planName, listName);
                    });
                    return true;
                }, false);
                info.row->add_controller(dropTarget);
            };

            listsBox->append(*listSection);
        };
        break;
    };
};
