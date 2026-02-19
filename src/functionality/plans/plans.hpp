#include <gtkmm/menubutton.h>

#include <nlohmann/json_fwd.hpp>
using json = nlohmann::json;

Gtk::MenuButton* makePlanButton();
void addPlanToJSON(json& data, const std::string& name);
void deletePlanFromJSON(json& data, const std::string& name);
