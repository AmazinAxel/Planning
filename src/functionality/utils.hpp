#include <nlohmann/json_fwd.hpp>
#include <cstdlib>
#include <string>
using json = nlohmann::json;

json initLoadJSON();
void saveJSON(const json& json);
bool downloadDataFromServer();
void uploadDataToServer();
