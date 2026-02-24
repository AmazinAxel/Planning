#pragma once
#include <gtkmm/application.h>
#include <gtkmm/box.h>
#include <gtkmm/menubutton.h>
#include <gtkmm/stack.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class PlanListPage: public Gtk::Box {
public:
    PlanListPage(json& appData);
    void refresh();
    json& appData;
    Gtk::Box* listBox = nullptr;
    Gtk::MenuButton* addPlanBtn = nullptr;
    void addPlan(const std::string& name);
};

class App: public Gtk::Application {
public:
    static App* get();
    static App* instance;
    PlanListPage* listPage = nullptr;
    Gtk::Stack* stack = nullptr;
    bool isSynced;
    json appData;
    std::string focusedList;
    int focusedEntryID = -1;
    std::string dragPlanName;
    std::string dragListName;
    int dragEntryID = -1;
    void openPlan(const Glib::ustring& name, const std::string& focusedList = "");
    void on_activate() override;
};

Gtk::Box* planPage(Gtk::Stack* stack, json& appData, const Glib::ustring& planName);
