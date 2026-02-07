#pragma once
#include <gtkmm.h>
#include <functional>

class PlansListView: public Gtk::Box {
public:
    PlansListView();

    std::function<void(const std::string&)> on_plan_selected;
};

Gtk::ScrolledWindow* planList();
