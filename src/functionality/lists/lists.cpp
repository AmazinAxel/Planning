#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/menubutton.h>
#include <gtkmm/popover.h>

#include "../../app.hpp"
#include "../utils.hpp"

static json* findPlan(json& data, const std::string& planName) {
    for (auto& plan: data["plans"]) {
        if (plan.contains(planName))
            return &plan[planName];
    };
    return nullptr;
};

static int nextEntryId(const json& entries) {
    int maxId = 0;
    for (auto& e: entries) {
        int id = e["id"].get<int>();
        if (id > maxId)
            maxId = id;
    };
    return maxId + 1;
};

void addListToPlanJSON(json& data, const std::string& planName, const std::string& listName) {
    auto* plan = findPlan(data, planName);
    (*plan)[listName] = {{"entries", json::array()}};
};

void addEntryToListJSON(json& data, const std::string& planName, const std::string& listName, const std::string& value) {
    auto* plan = findPlan(data, planName);
    auto& entries = (*plan)[listName]["entries"];
    int id = nextEntryId(entries);
    entries.push_back({{"id", id}, {"value", value}});
};

void deleteListFromPlanJSON(json& data, const std::string& planName, const std::string& listName) {
    auto* plan = findPlan(data, planName);
    plan->erase(listName);
};

void deleteEntryFromListJSON(json& data, const std::string& planName, const std::string& listName, int entryId) {
    auto* plan = findPlan(data, planName);
    auto& entries = (*plan)[listName]["entries"];
    auto it = std::remove_if(entries.begin(), entries.end(),
        [entryId](const json& e) { return e["id"].get<int>() == entryId; });
    entries.erase(it, entries.end());
};

void editEntryInListJSON(json& data, const std::string& planName, const std::string& listName, int entryId, const std::string& newValue) {
    auto* plan = findPlan(data, planName);
    auto& entries = (*plan)[listName]["entries"];
    for (auto& e: entries) {
        if (e["id"].get<int>() == entryId) {
            e["value"] = newValue;
            return;
        };
    };
};

Gtk::MenuButton* makeListButton(const std::string& planName, std::function<void()> refreshCallback) {
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
    sendButton->set_icon_name("mail-reply-sender-symbolic");
    hbox->append(*sendButton);

    popover->set_autohide(false);
    popover->signal_show().connect([entry]() { entry->grab_focus(); });

    auto submit = [entry, popover, planName, refreshCallback]() {
        auto listName = entry->get_text();
        if (!listName.empty()) {
            addListToPlanJSON(App::get()->appData, planName, listName);
            saveJSON(App::get()->appData);
            entry->set_text("");
            popover->popdown();
            refreshCallback();
        };
    };
    entry->signal_activate().connect(submit);
    sendButton->signal_clicked().connect(submit);

    return button;
};
