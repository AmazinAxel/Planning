#pragma once
#include <gtkmm.h>
#include <functional>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

Gtk::Box* planListPage(json& appData, std::function<void(const Glib::ustring&)> onSelect);

class PlanPage: public Gtk::Box {
    public:
        PlanPage(std::function<void()> onBack);
        void setPlanName(const Glib::ustring& name);
    private:
        Gtk::Label* titleLabel = nullptr;
};
