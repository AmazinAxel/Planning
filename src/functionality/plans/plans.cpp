#include <gtkmm.h>
#include <iostream>
#include "../../app.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

void addPlanToJSON(json& data, const std::string& name) {
    data["plans"].push_back({
        {"name", name},
        {"lists", json::array()}
    });
};

Gtk::MenuButton* makePlanButton() {
    auto button = Gtk::make_managed<Gtk::MenuButton>();
    button->set_icon_name("list-add-symbolic");
    auto popover = Gtk::make_managed<Gtk::Popover>();
    button->set_popover(*popover);

    auto hbox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 6);
    popover->set_child(*hbox);

    auto entry = Gtk::make_managed<Gtk::Entry>();
    entry->set_placeholder_text("New plan name");
    entry->set_hexpand(true);
    hbox->append(*entry);

    auto sendButton = Gtk::make_managed<Gtk::Button>();
    sendButton->set_icon_name("mail-forward-symbolic");
    hbox->append(*sendButton);

    popover->set_autohide(false); // fix weird close instantly issue

    // Create new plan
    auto submit = [entry, popover]() {
        auto planName = entry->get_text();
        if (!planName.empty()) {
            App::get()->listPage->addPlan(planName);
            entry->set_text("");
            popover->popdown();
        };
    };
    entry->signal_activate().connect(submit);
    sendButton->signal_clicked().connect(submit);

    return button;
};

void deletePlanFromJSON(json& data, const std::string& name) {
    auto& arr = data["plans"];

    auto it = std::remove_if(arr.begin(), arr.end(),
        [&](const json& p) {
            return p["name"].get<std::string>() == name;
        }
    );
    arr.erase(it, arr.end());
};

// Update this function to match the new schema:
void addPlanToJSON(json& data, const std::string& name) {
    json newPlan;
    newPlan["plan_name"] = json::object();
    data["plans"].push_back(newPlan);
}

// Update delete to use index instead of name:
void deletePlanFromJSON(json& data, int planIndex) {
    auto& arr = data["plans"];
    if (planIndex >= 0 && planIndex < arr.size()) {
        arr.erase(arr.begin() + planIndex);
    }
}