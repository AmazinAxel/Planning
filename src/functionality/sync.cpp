#include <glibmm.h> // todo
#include <nlohmann/json.hpp>
#include <string>
#include <iostream>

using json = nlohmann::json;

// If sync config is set, return it
static bool getSyncConfig(json& config) {
    auto data_path = Glib::get_user_config_dir() + "/planning/data.json";

    // This is called before the config file is generated
    if (!Glib::file_test(data_path, Glib::FileTest::EXISTS))
        return false;

    std::string contents = Glib::file_get_contents(data_path);
    config = json::parse(contents, nullptr, false);
    return config.contains("smbserver");
};

bool downloadDataFromServer() {
    json config;
    if (!getSyncConfig(config)) return false;

    std::string server = config["smbserver"];
    std::string user     = config.value("user", "");
    std::string group    = config.value("group", "workgroup");
    std::string password = config.value("password", "");
    std::string share    = config.value("smbshare", "");

    auto data_path = Glib::get_user_config_dir() + "/planning/data.json";

    std::vector<std::string> argv = {
        "smbclient", "//" + server + "/" + share,
        "-U", user + "%" + password,
        "-W", group,
        "-c", "get planningData.json " + data_path
    };

    try {
        std::string out, err;
        int status;
        Glib::spawn_sync("", argv, Glib::SpawnFlags::SEARCH_PATH, {}, &out, &err, &status);
        if (status != 0) {
            std::cerr << "Download from server failed: " << out << err << std::endl;
            return false;
        };
    } catch (const Glib::Error& err) {
        std::cerr << "Download from server error: " << err.what() << std::endl;
        return false;
    };
    return true;
};

void uploadDataToServer() {
    json config;
    if (!getSyncConfig(config)) return;

    std::string server   = config["smbserver"];
    std::string user     = config.value("user", "");
    std::string group    = config.value("group", "workgroup");
    std::string password = config.value("password", "");
    std::string share    = config.value("smbshare", "");

    auto data_path = Glib::get_user_config_dir() + "/planning/data.json";

    std::vector<std::string> argv = {
        "smbclient", "//" + server + "/" + share,
        "-U", user + "%" + password,
        "-W", group,
        "-c", "put " + data_path + " planningData.json"
    };

    try {
        std::string out, err;
        int status;
        Glib::spawn_sync("", argv, Glib::SpawnFlags::SEARCH_PATH, {}, &out, &err, &status);
        if (status != 0)
            std::cerr << "Upload to server failed: " << out << err << std::endl;
    } catch (const Glib::Error& err) {
        std::cerr << "Upload to server error: " << err.what() << std::endl;
    };
};
