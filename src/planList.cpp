#include "app.hpp"
#include "functionality/plans/plans.hpp"
#include "functionality/utils.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

Gtk::Button* planItem(const Glib::ustring& name, std::function<void(const Glib::ustring&)> onSelect) {
    auto btn = Gtk::make_managed<Gtk::Button>(name);
    btn->add_css_class("planName");

    btn->signal_clicked().connect([name, onSelect]() {
        onSelect(name);
    });

    return btn;
};

void updatePlanList(Gtk::Box* listBox, json& appData, std::function<void(const Glib::ustring&)> onSelect) {
    while (auto child = listBox->get_first_child())
        listBox->remove(*child); // Remove previous plans

    // Add all plans 
    for (auto& plan : appData["plans"]) {
        Glib::ustring name = plan["name"].get<std::string>();
        listBox->append(*planItem(name, onSelect));
    };
};

Gtk::Box* planListPage(json& appData, std::function<void(const Glib::ustring&)> onSelect) {
    auto planListParent = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    auto listBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);

    // Header
    auto header = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
    header->add_css_class("appHeader");
    header->set_hexpand(true);

    auto title = Gtk::make_managed<Gtk::Label>("Planning");
    title->set_halign(Gtk::Align::START);
    title->add_css_class("headerText");
    title->set_hexpand(true);

    header->append(*title);
    header->append(*makePlanButton( // Create new plan butto
        [listBox, &appData, onSelect](const std::string& name) {
            addPlanToJSON(appData, name);
            saveJSON(appData);
            updatePlanList(listBox, appData, onSelect);
        }
    ));
    planListParent->append(*header);

    // Plan items
    listBox->set_spacing(10);
    listBox->set_vexpand(true);

    updatePlanList(listBox, appData, onSelect);

    auto scroll = Gtk::make_managed<Gtk::ScrolledWindow>();
    scroll->set_child(*listBox);
    planListParent->append(*scroll);

    return planListParent;
};
