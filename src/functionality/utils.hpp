#include <nlohmann/json.hpp>
using json = nlohmann::json;

json initLoadJSON();
void save_json(const json& json);
