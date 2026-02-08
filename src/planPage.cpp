#include "app.hpp"
#include "functionality/plans/plans.hpp"
#include "functionality/utils.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

Gtk::Box* planPage(Gtk::Stack* stack, json& appData, const Glib::ustring& planName) {
    auto planPage = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    // Header
    auto header = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);

    auto backBtn = Gtk::make_managed<Gtk::Button>();
    auto backIcon = Gtk::make_managed<Gtk::Image>();
    backIcon->set_from_icon_name("go-previous-symbolic");
    backBtn->set_child(*backIcon);

    // Go back
    backBtn->signal_clicked().connect([stack]() { stack->set_visible_child("list"); }); 

    auto titleLabel = Gtk::make_managed<Gtk::Label>(planName);
    titleLabel->set_halign(Gtk::Align::END);
    titleLabel->add_css_class("headerText");
    titleLabel->set_hexpand(true);

    header->append(*backBtn);
    header->append(*titleLabel);

    planPage->append(*header);

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
            //updatePlanList(listBox, appData, onSelect);

            return true;
        };

        return false;
    }, false);

    // APPEND(*) STUFF HERE

    return planPage;
};
