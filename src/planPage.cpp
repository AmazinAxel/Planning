#include "app.hpp"

PlanPage::PlanPage(std::function<void()> onBack): Gtk::Box(Gtk::Orientation::VERTICAL) {
    // Header
    auto header = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);

    auto backBtn = Gtk::make_managed<Gtk::Button>();
    auto backIcon = Gtk::make_managed<Gtk::Image>();
    backIcon->set_from_icon_name("go-previous-symbolic");
    backBtn->set_child(*backIcon);

    backBtn->signal_clicked().connect([onBack]() {
        onBack();
    });

    titleLabel = Gtk::make_managed<Gtk::Label>("Plan");
    titleLabel->set_halign(Gtk::Align::END);
    titleLabel->add_css_class("headerText");
    titleLabel->set_hexpand(true);

    header->append(*backBtn);
    header->append(*titleLabel);

    append(*header);

    // APPEND(*) STUFF HERE
}

void PlanPage::setPlanName(const Glib::ustring& name) {
    titleLabel->set_text(name);
}