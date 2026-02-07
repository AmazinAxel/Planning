#include <gtkmm.h>

Gtk::ScrolledWindow* planList() {
    auto box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    box->set_margin(6);

    auto header = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
    header->set_hexpand(true);

    auto spacer = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
    spacer->set_hexpand(true);

    auto plus_button = Gtk::make_managed<Gtk::Button>("+");
    plus_button->set_halign(Gtk::Align::END);
    plus_button->set_valign(Gtk::Align::START);

    header->append(*spacer);
    header->append(*plus_button);

    box->append(*header);

    auto scrolled = Gtk::make_managed<Gtk::ScrolledWindow>();
    scrolled->set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
    scrolled->set_child(*box);

    return scrolled;
}
