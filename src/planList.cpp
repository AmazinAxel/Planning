#include <gtkmm.h>
#include "app.hpp"

Gtk::ScrolledWindow* planList() {
    auto listBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    listBox->set_spacing(6);

    auto header = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
    header->add_css_class("appHeader");
    header->set_hexpand(true);

    auto headerText = Gtk::make_managed<Gtk::Label>("Planning");
    headerText->set_hexpand(true);
    headerText->set_halign(Gtk::Align::START); // Left align
    headerText->add_css_class("headerText");

    // Add list button
    auto addListButton = Gtk::make_managed<Gtk::Button>();
    addListButton->set_halign(Gtk::Align::END);
    auto addListIcon = Gtk::make_managed<Gtk::Image>();
    addListIcon->set_from_icon_name("list-add-symbolic");
    addListButton->set_child(*addListIcon);

    header->append(*headerText);
    header->append(*addListButton);

    listBox->append(*header);
    listBox->append(*planItem("Math"));
    listBox->append(*planItem("General"));
    listBox->append(*planItem("Hack Club"));

    // Scrolled window
    auto scrolled = Gtk::make_managed<Gtk::ScrolledWindow>();
    scrolled->set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
    scrolled->set_child(*listBox);

    return scrolled;
};

Gtk::Button* planItem(const Glib::ustring& planNameText) {
    auto planItem = Gtk::make_managed<Gtk::Button>(planNameText);
    planItem->add_css_class("planName");
    planItem->set_margin(6);
    return planItem;
}
