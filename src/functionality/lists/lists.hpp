#include <gtkmm/menubutton.h>
#include <gtkmm/box.h>
#include <vector>
#include <string>
#include <functional>

#include <nlohmann/json_fwd.hpp>
using json = nlohmann::json;

int nextEntryId(const json& entries);
int addListToPlanJSON(json& data, const std::string& planName, const std::string& listName);
int addEntryToListJSON(json& data, const std::string& planName, const std::string& listName, int afterID);
void deleteListFromPlanJSON(json& data, const std::string& planName, const std::string& listName);
void deleteEntryFromListJSON(json& data, const std::string& planName, const std::string& listName, int entryId);
void editEntryInListJSON(json& data, const std::string& planName, const std::string& listName, int entryId, const std::string& newValue);
void setEntryIndentInListJSON(json& data, const std::string& planName, const std::string& listName, int entryId, bool isIndented);
void ensureListNotEmpty(json& data, const std::string& planName, const std::string& listName);
void reorderListsInJSON(json& data, const std::string& planName, const std::vector<std::string>& newOrder);
void moveEntryJSON(json& data, const std::string& fromPlan, const std::string& fromList, int entryId, const std::string& toPlan, const std::string& toList, int afterId);

Gtk::MenuButton* makeListButton(const std::string& planName, std::function<void()> refreshCallback);
