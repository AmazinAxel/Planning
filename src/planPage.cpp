#include <gtkmm/button.h>
#include <gtkmm/eventcontrollerkey.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/gesturedrag.h>
#include <gtkmm/scrolledwindow.h>
#include <memory>
#include <gdk/gdkkeysyms.h>

#include "app.hpp"
#include "functionality/plans/plans.hpp"
#include "functionality/lists/lists.hpp"
#include "functionality/utils.hpp"

Gtk::Box* planPage(Gtk::Stack* stack, json& appData, const Glib::ustring& planName) {
    auto planPage = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    planPage->add_css_class("planPage");
    auto header = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
    header->add_css_class("appHeader");

    // Go back button
    auto backBtn = Gtk::make_managed<Gtk::Button>();
    auto backIcon = Gtk::make_managed<Gtk::Image>();
    backIcon->set_from_icon_name("go-previous-symbolic");
    backBtn->set_child(*backIcon);
    backBtn->signal_clicked().connect([stack]() { stack->set_visible_child("list"); });

    // Plan name
    auto planNameLabel = Gtk::make_managed<Gtk::Label>(planName);
    planNameLabel->add_css_class("headerText");
    planNameLabel->set_hexpand(true);

    header->append(*backBtn);
    header->append(*planNameLabel);

    // Scrollable container for lists (horizontal)
    auto listsBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 12);
    listsBox->set_margin_start(12);
    listsBox->set_margin_end(12);
    listsBox->set_margin_top(8);
    listsBox->set_margin_bottom(8);
    listsBox->set_spacing(20); // 20px gap between lists

    auto scroll = Gtk::make_managed<Gtk::ScrolledWindow>();
    scroll->set_child(*listsBox);
    scroll->set_vexpand(true);
    scroll->set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);

    // Drag to scroll
    auto drag = Gtk::GestureDrag::create();
    auto startH = std::make_shared<double>(0.0);
    auto startV = std::make_shared<double>(0.0);
    drag->signal_drag_begin().connect([scroll, startH, startV](double, double) {
        *startH = scroll->get_hadjustment()->get_value();
        *startV = scroll->get_vadjustment()->get_value();
    });
    drag->signal_drag_update().connect([scroll, startH, startV](double dx, double dy) {
        scroll->get_hadjustment()->set_value(*startH - dx);
        scroll->get_vadjustment()->set_value(*startV - dy);
    });
    scroll->add_controller(drag);

    // Add list button (in header)
    auto addListBtn = makeListButton(planName, [planName]() {
        App::get()->openPlan(planName);
    });
    header->append(*addListBtn);

    planPage->append(*header);
    planPage->append(*scroll);

    // Escape and plan page deletion
    auto key = Gtk::EventControllerKey::create();
    planPage->add_controller(key);
    key->signal_key_pressed().connect([stack, planName, &appData](guint keyval, guint keycode, Gdk::ModifierType state) {
        if (keyval == GDK_KEY_Escape) {
            stack->set_visible_child("list"); // Go back
            return true;
        };

        // Delete current page
        bool ctrl = static_cast<bool>(state & Gdk::ModifierType::CONTROL_MASK); // is holding control
        if (ctrl && keyval == GDK_KEY_d) {
            stack->set_visible_child("list"); // Go back
            deletePlanFromJSON(appData, planName);
            saveJSON(appData);
            App::get()->listPage->refresh();
            return true;
        };

        return false;
    }, false);

    renderLists(listsBox, appData, planName);

    return planPage;
};
