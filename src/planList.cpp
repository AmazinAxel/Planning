#include <gtkmm/button.h>
#include <gtkmm/eventcontrollerkey.h>
#include <gtkmm/label.h>
#include <gtkmm/scrolledwindow.h>
#include <gdk/gdkkeysyms.h>

#include "app.hpp"
#include "functionality/plans/plans.hpp"
#include "functionality/utils.hpp"

PlanListPage::PlanListPage(json& data): Gtk::Box(Gtk::Orientation::VERTICAL), appData(data) {
    // Header
    auto header = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
    header->add_css_class("appHeader");
    header->add_css_class("planListHeader");
    auto title = Gtk::make_managed<Gtk::Label>("Planning");
    title->add_css_class("headerText");
    title->set_hexpand(true);
    header->append(*title);
    append(*header);

    // List box
    listBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    listBox->set_spacing(10);
    listBox->set_margin_top(5); // Fix outline cut off
    listBox->set_vexpand(true);
    auto scroll = Gtk::make_managed<Gtk::ScrolledWindow>();
    scroll->set_child(*listBox);
    append(*scroll);

    // Sync status
    auto syncLabel = Gtk::make_managed<Gtk::Label>(App::get()->isSynced ? "Synced" : "Not synced");
    syncLabel->set_halign(Gtk::Align::START);
    syncLabel->add_css_class("isSynced");
    append(*syncLabel);

    // control + N to open add plan popover
    auto key = Gtk::EventControllerKey::create();
    add_controller(key);
    key->signal_key_pressed().connect([this](guint keyval, guint, Gdk::ModifierType state) {
        bool ctrl = static_cast<bool>(state & Gdk::ModifierType::CONTROL_MASK);
        if (ctrl && keyval == GDK_KEY_n)
            addPlanBtn->popup();
        return false;
    }, false);

    // Add all plans
    refresh();
};

void PlanListPage::addPlan(const std::string& name) {
    addPlanToJSON(appData, name);
    saveJSON(appData);
    refresh();
};

void PlanListPage::refresh() {
    while (auto child = listBox->get_first_child())
        listBox->remove(*child);

    for (auto& plan: appData["plans"]) {
        Glib::ustring name = plan.begin().key();
        auto btn = Gtk::make_managed<Gtk::Button>(name);
        btn->add_css_class("planName");
        btn->set_halign(Gtk::Align::CENTER);

        btn->signal_clicked().connect([name]() {
            App::get()->openPlan(name);
        });

        listBox->append(*btn);
    };
    addPlanBtn = makePlanButton();
    listBox->append(*addPlanBtn); // Create new plan button
};
