#include <gtkmm/menubutton.h>
#include <gtkmm/box.h>

#include <nlohmann/json_fwd.hpp>
using json = nlohmann::json;

int addListToPlanJSON(json& data, const std::string& planName, const std::string& listName);
int addEntryToListJSON(json& data, const std::string& planName, const std::string& listName, int afterID);
void deleteListFromPlanJSON(json& data, const std::string& planName, const std::string& listName);
void deleteEntryFromListJSON(json& data, const std::string& planName, const std::string& listName, int entryId);
void editEntryInListJSON(json& data, const std::string& planName, const std::string& listName, int entryId, const std::string& newValue);
void setEntryIndentInListJSON(json& data, const std::string& planName, const std::string& listName, int entryId, bool isIndented);
void moveEntryJSON(json& data, const std::string& fromPlan, const std::string& fromList, int entryId, const std::string& toPlan, const std::string& toList, int afterId);
void renderLists(Gtk::Box* listsBox, json& appData, const std::string& planName);

Gtk::MenuButton* makeListButton(const std::string& planName, std::function<void()> refreshCallback);
