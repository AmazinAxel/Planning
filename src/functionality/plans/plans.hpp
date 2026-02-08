#include <gtkmm.h> // todo

#include <nlohmann/json.hpp>
using json = nlohmann::json;

Gtk::MenuButton* makePlanButton(std::function<void(const std::string&)> onCreate);
void addPlanToJSON(json& data, const std::string& name);
void deletePlanFromJSON(json& data, const std::string& name);
