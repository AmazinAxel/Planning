#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/menubutton.h>
#include <gtkmm/popover.h>

#include "../../app.hpp"

void addPlanToJSON(json& data, const std::string& name) {
    json newPlan;
    newPlan[name] = json::object();
    data["plans"].push_back(newPlan);
};

Gtk::MenuButton* makePlanButton() {
    auto button = Gtk::make_managed<Gtk::MenuButton>();
    button->set_icon_name("list-add-symbolic");
    button->set_halign(Gtk::Align::CENTER);
    button->add_css_class("addNewPlanButton");

    auto popover = Gtk::make_managed<Gtk::Popover>();
    button->set_popover(*popover);

    auto hbox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 6);
    popover->set_child(*hbox);

    auto entry = Gtk::make_managed<Gtk::Entry>();
    entry->set_placeholder_text("New plan name");
    entry->set_hexpand(true);
    hbox->append(*entry);

    auto sendButton = Gtk::make_managed<Gtk::Button>();
    sendButton->set_icon_name("mail-reply-sender-symbolic");
    sendButton->add_css_class("addNewPlanPopoverButton");
    hbox->append(*sendButton);

    popover->set_autohide(false); // fix weird close instantly issue
    popover->signal_show().connect([entry]() { entry->grab_focus(); });

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
            return p.contains(name);
        }
    );
    arr.erase(it, arr.end());
};
