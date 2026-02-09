#include <gtkmm.h> // todo
#include <functional>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

void addListToPlanJSON(json& data, const std::string& planName, const std::string& listName);
void addEntryToListJSON(json& data, const std::string& planName, const std::string& listName, const std::string& value);
void deleteListFromPlanJSON(json& data, const std::string& planName, const std::string& listName);
void deleteEntryFromListJSON(json& data, const std::string& planName, const std::string& listName, int entryId);
void editEntryInListJSON(json& data, const std::string& planName, const std::string& listName, int entryId, const std::string& newValue);

Gtk::MenuButton* makeListButton(const std::string& planName, std::function<void()> refreshCallback);
