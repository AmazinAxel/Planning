#include <gtkmm.h> // todo
#include <iostream>

Gtk::MenuButton* makeListButton() {
    auto button = Gtk::make_managed<Gtk::MenuButton>();
    button->set_icon_name("list-add-symbolic");

    auto popover = Gtk::make_managed<Gtk::Popover>();
    button->set_popover(*popover);

    // Input & submit
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

    return button;
};
