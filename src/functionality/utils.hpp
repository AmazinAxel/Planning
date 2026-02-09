#include <nlohmann/json.hpp>
using json = nlohmann::json;

json initLoadJSON();
void saveJSON(const json& json);
void downloadDataFromServer();
void uploadDataToServer();
