#include <nlohmann/json.hpp>
using json = nlohmann::json;

void deletePlanFromJSON(json& data, const std::string& name) {
    auto& arr = data["plans"];

    auto it = std::remove_if(arr.begin(), arr.end(),
        [&](const json& p) {
            return p["name"].get<std::string>() == name;
        }
    );
    arr.erase(it, arr.end());
};
