#include "app.hpp"

Gtk::ScrolledWindow* planList(std::function<void(const Glib::ustring&)> onSelect) {
    auto box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    box->set_spacing(10);

    // Header
    auto header = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
    header->add_css_class("appHeader");
    header->set_hexpand(true);

    auto title = Gtk::make_managed<Gtk::Label>("Planning");
    title->set_halign(Gtk::Align::START);
    title->add_css_class("headerText");
    title->set_hexpand(true);

    auto addBtn = Gtk::make_managed<Gtk::Button>();
    auto icon = Gtk::make_managed<Gtk::Image>();
    icon->set_from_icon_name("list-add-symbolic");
    addBtn->set_child(*icon);

    header->append(*title);
    header->append(*addBtn);

    box->append(*header);

    // demo items TODO replace with dynamic
    box->append(*planItem("Math", onSelect));
    box->append(*planItem("General", onSelect));
    box->append(*planItem("Hack Club", onSelect));

    // Scrollable
    auto scrollable = Gtk::make_managed<Gtk::ScrolledWindow>();
    scrollable->set_child(*box);

    return scrollable;
};

static Gtk::Button* planItem(const Glib::ustring& name, std::function<void(const Glib::ustring&)> onSelect) {
    auto btn = Gtk::make_managed<Gtk::Button>(name);
    btn->add_css_class("planName");

    btn->signal_clicked().connect([name, onSelect]() {
        onSelect(name);
    });

    return btn;
};
