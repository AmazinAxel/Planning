#include "app.hpp"
#include "functionality/plans/plans.hpp"

Gtk::Box* planList(std::function<void(const Glib::ustring&)> onSelect) {
    auto planListParent = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);

    // Header
    auto header = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
    header->add_css_class("appHeader");
    header->set_hexpand(true);

    auto title = Gtk::make_managed<Gtk::Label>("Planning");
    title->set_halign(Gtk::Align::START);
    title->add_css_class("headerText");
    title->set_hexpand(true);

    header->append(*title);
    header->append(*makeListButton());

    planListParent->append(*header);

    // demo items TODO replace with dynamic
    auto planList = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    planList->set_spacing(10);
    planList->set_vexpand(true);

    planList->append(*planItem("Math", onSelect));
    planList->append(*planItem("General", onSelect));
    planList->append(*planItem("Hack Club", onSelect));

    // Scrollable
    auto scrollable = Gtk::make_managed<Gtk::ScrolledWindow>();
    scrollable->set_child(*planList);
    planListParent->append(*scrollable);

    return planListParent;
};

Gtk::Button* planItem(const Glib::ustring& name, std::function<void(const Glib::ustring&)> onSelect) {
    auto btn = Gtk::make_managed<Gtk::Button>(name);
    btn->add_css_class("planName");

    btn->signal_clicked().connect([name, onSelect]() {
        onSelect(name);
    });

    return btn;
};
