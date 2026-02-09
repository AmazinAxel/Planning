#include <nlohmann/json.hpp>
using json = nlohmann::json;

json initLoadJSON();
void saveJSON(const json& json);
bool downloadDataFromServer();
void uploadDataToServer();
