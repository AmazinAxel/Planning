#include "app.hpp"
#include "functionality/plans/plans.hpp"
#include "functionality/utils.hpp"

PlanListPage::PlanListPage(json& data): Gtk::Box(Gtk::Orientation::VERTICAL), appData(data) {
    // Header
    auto header = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
    header->add_css_class("appHeader");

    auto title = Gtk::make_managed<Gtk::Label>("Planning");
    title->add_css_class("headerText");
    title->set_hexpand(true);

    header->append(*title);

    // Create new plan button
    header->append(*makePlanButton());
    append(*header);

    // List box
    listBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    listBox->set_spacing(10);
    listBox->set_vexpand(true);
    auto scroll = Gtk::make_managed<Gtk::ScrolledWindow>();
    scroll->set_child(*listBox);
    append(*scroll);

    // Sync status
    auto syncLabel = Gtk::make_managed<Gtk::Label>(App::get()->isSynced ? "Synced" : "Not synced");
    syncLabel->set_halign(Gtk::Align::START);
    syncLabel->add_css_class("isSynced");
    append(*syncLabel);

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

        btn->signal_clicked().connect([name]() {
            App::get()->openPlan(name);
        });

        listBox->append(*btn);
    };
};
