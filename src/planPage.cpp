#include "app.hpp"
#include "functionality/plans/plans.hpp"

PlanPage::PlanPage(Gtk::Stack* stack): Gtk::Box(Gtk::Orientation::VERTICAL) {
    // Header
    auto header = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);

    auto backBtn = Gtk::make_managed<Gtk::Button>();
    auto backIcon = Gtk::make_managed<Gtk::Image>();
    backIcon->set_from_icon_name("go-previous-symbolic");
    backBtn->set_child(*backIcon);

    // Go back
    backBtn->signal_clicked().connect([stack]() { stack->set_visible_child("list"); }); 

    titleLabel = Gtk::make_managed<Gtk::Label>("Plan");
    titleLabel->set_halign(Gtk::Align::END);
    titleLabel->add_css_class("headerText");
    titleLabel->set_hexpand(true);

    header->append(*backBtn);
    header->append(*titleLabel);

    append(*header);

    // Escape and plan page deletion
    auto key = Gtk::EventControllerKey::create();
    add_controller(key);
    key->signal_key_pressed().connect([stack](guint keyval, guint keycode, Gdk::ModifierType state) {
        if (keyval == GDK_KEY_Escape) {
            stack->set_visible_child("list"); // Go back
            return true;
        };

        // Delete current page
        bool ctrl = static_cast<bool>(state & Gdk::ModifierType::CONTROL_MASK); // is holding control
        if (ctrl && keyval == GDK_KEY_d) {
            stack->set_visible_child("list"); // Go back
            deletePlanFromJSON();
            return true;
        };

        return false;
    }, false);


    // APPEND(*) STUFF HERE
};
