#pragma once
#include <gtkmm.h>
#include <functional>

struct PlanList {
    Gtk::ScrolledWindow* scrolled;
    Gtk::Box* listBox;
};

class PlansListView: public Gtk::Box {
public:
    PlansListView();

    std::function<void(const std::string&)> on_plan_selected;
};

PlanList createPlanList();
Gtk::Button* planItem(const Glib::ustring& title_text);
