#pragma once
#include <gtkmm.h>
#include <functional>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class PlanListPage: public Gtk::Box {
public:
    PlanListPage(json& appData);
    void refresh();
    json& appData;
    Gtk::Box* listBox = nullptr;
    void addPlan(const std::string& name);
};

class App: public Gtk::Application {
public:
    static App* get();
    static App* instance;
    PlanListPage* listPage = nullptr;
    Gtk::Stack* stack = nullptr;
    json appData;
    void openPlan(const Glib::ustring& name);
    void on_activate() override;
};

Gtk::Box* planPage(Gtk::Stack* stack, json& appData, const Glib::ustring& planName);
