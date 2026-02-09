#include <gtkmm.h> // todo

#include <nlohmann/json.hpp>
using json = nlohmann::json;

void addListToPlanJSON(json& data, int planIndex, const std::string& listName);

void addEntryToListJSON(json& data, int planIndex, const std::string& listName, int id, const std::string& value);

Gtk::MenuButton* makeListButton(int planIndex);

void deleteListFromPlanJSON(json& data, int planIndex, const std::string& listName);
void deleteEntryFromListJSON(json& data, int planIndex, const std::string& listName, int id);
