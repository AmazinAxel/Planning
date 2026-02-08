#include <gtkmm.h>
#include <iostream>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

void addPlanToJSON(json& data, const std::string& name) {
    data["plans"].push_back({
        {"name", name},
        {"lists", json::array()}
    });
};

Gtk::MenuButton* makePlanButton(std::function<void(const std::string&)> onCreate) {
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

    // Callback when submitted
    auto submit = [entry, popover, onCreate]() {
        auto text = entry->get_text();
        if (!text.empty()) {
            onCreate(text);
            entry->set_text("");
            popover->popdown();
        };
    };
    entry->signal_activate().connect(submit);
    sendButton->signal_clicked().connect(submit);

    return button;
};
