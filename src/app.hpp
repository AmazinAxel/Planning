#pragma once
#include <gtkmm.h>
#include <functional>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

Gtk::Box* planListPage(json& appData, std::function<void(const Glib::ustring&)> onSelect);
void updatePlanList(Gtk::Box* listBox, json& appData, std::function<void(const Glib::ustring&)> onSelect);

Gtk::Box* planPage(Gtk::Stack* stack, json& appData, const Glib::ustring& planName);
