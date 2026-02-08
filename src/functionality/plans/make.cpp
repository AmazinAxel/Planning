#include <gtkmm.h> // todo
#include <iostream>
Gtk::MenuButton* makeListButton() {
    auto button = Gtk::make_managed<Gtk::MenuButton>();
    button->set_icon_name("list-add-symbolic");

    auto popover = Gtk::make_managed<Gtk::PopoverMenu>();
    button->set_popover(*popover);

    auto vbox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL, 6);
    popover->set_child(*vbox);


    auto label = Gtk::make_managed<Gtk::Label>("eee");
    //auto entry = Gtk::make_managed<Gtk::Entry>();
    //entry->set_placeholder_text("New list name…");
    vbox->append(*label);

    auto createBtn = Gtk::make_managed<Gtk::Button>("Create");
    vbox->append(*createBtn);

    return button;
};
