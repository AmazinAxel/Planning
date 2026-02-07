#include <gtkmm.h>
#include "app.hpp"

PlanList createPlanList() {
    auto listBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    listBox->set_spacing(6);

    auto header = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
    header->set_hexpand(true);

    auto spacer = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
    spacer->set_hexpand(true);

    auto plus_button = Gtk::make_managed<Gtk::Button>("+");
    plus_button->set_halign(Gtk::Align::END);

    header->append(*spacer);
    header->append(*plus_button);

    listBox->append(*header);

    // Scrolled window
    auto scrolled = Gtk::make_managed<Gtk::ScrolledWindow>();
    scrolled->set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
    scrolled->set_child(*listBox);

    return { scrolled, listBox };
};

Gtk::Button* planItem(const Glib::ustring& planNameText) {
    auto planItem = Gtk::make_managed<Gtk::Button>(planNameText);
    planItem->add_css_class("planName");
    planItem->set_margin(6);
    return planItem;
}
