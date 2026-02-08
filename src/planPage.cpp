#include "app.hpp"
#include "functionality/plans/plans.hpp"
#include "functionality/utils.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

Gtk::Box* planPage(Gtk::Stack* stack, json& appData, const Glib::ustring& planName) {
    auto planPage = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    auto header = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);

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

    // Add list button
    auto addListBtn = Gtk::make_managed<Gtk::Button>();
    auto addIcon = Gtk::make_managed<Gtk::Image>();
    addIcon->set_from_icon_name("list-add-symbolic");
    addListBtn->set_child(*addIcon);
    addListBtn->signal_clicked().connect([stack]() { stack->set_visible_child("list"); });

    header->append(*backBtn);
    header->append(*planNameLabel);
    header->append(*addListBtn);

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
            App::get()->listPage->refresh();
            return true;
        };

        return false;
    }, false);

    // APPEND(*) STUFF HERE

    return planPage;
};
