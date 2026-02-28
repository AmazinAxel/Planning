#include <nlohmann/json_fwd.hpp>
#include <cstdlib>
#include <string>
using json = nlohmann::json;

inline bool isOnBroadway() {
    const char* backend = std::getenv("GDK_BACKEND");
    return backend && std::string(backend) == "broadway";
}

json initLoadJSON();
void saveJSON(const json& json);
bool downloadDataFromServer();
void uploadDataToServer();
