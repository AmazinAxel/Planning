#include <gtkmm.h> // todo
#include <iostream>
#include "../../app.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

void addListToPlanJSON(json& data, int planIndex, const std::string& listName) {
    auto& plan = data["plans"][planIndex]["plan_name"];
    plan[listName] = {
        {"entries", json::array()}
    };
}

void addEntryToListJSON(json& data, int planIndex, const std::string& listName, 
                        int id, const std::string& value) {
    auto& entries = data["plans"][planIndex]["plan_name"][listName]["entries"];
    entries.push_back({
        {"id", id},
        {"value", value}
    });
}

Gtk::MenuButton* makeListButton(int planIndex) {
    auto button = Gtk::make_managed<Gtk::MenuButton>();
    button->set_icon_name("list-add-symbolic");
    auto popover = Gtk::make_managed<Gtk::Popover>();
    button->set_popover(*popover);

    auto hbox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 6);
    popover->set_child(*hbox);

    auto entry = Gtk::make_managed<Gtk::Entry>();
    entry->set_placeholder_text("New list name");
    entry->set_hexpand(true);
    hbox->append(*entry);

    auto sendButton = Gtk::make_managed<Gtk::Button>();
    sendButton->set_icon_name("mail-forward-symbolic");
    hbox->append(*sendButton);

    popover->set_autohide(false);

    auto submit = [entry, popover, planIndex]() {
        auto listName = entry->get_text();
        if (!listName.empty()) {
            //App::get()->listPage->addListToPlan(planIndex, listName); // todo
            entry->set_text("");
            popover->popdown();
        }
    };
    entry->signal_activate().connect(submit);
    sendButton->signal_clicked().connect(submit);

    return button;
}

void deleteListFromPlanJSON(json& data, int planIndex, const std::string& listName) {
    auto& plan = data["plans"][planIndex]["plan_name"];
    if (plan.contains(listName)) {
        plan.erase(listName);
    }
}

void deleteEntryFromListJSON(json& data, int planIndex, const std::string& listName, int id) {
    auto& entries = data["plans"][planIndex]["plan_name"][listName]["entries"];
    
    auto it = std::remove_if(entries.begin(), entries.end(),
        [id](const json& entry) {
            return entry["id"].get<int>() == id;
        }
    );
    entries.erase(it, entries.end());
}