#include <glibmm/fileutils.h>
#include <glibmm/miscutils.h>
#include <glibmm/spawn.h>

#include <iostream>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "utils.hpp"

static const std::string BROADWAY_DATA_PATH = "/media/planningData.json";

// If samba sync config is set, return it
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
    auto local_path = Glib::get_user_config_dir() + "/planning/data.json";

    if (isOnBroadway()) { // Get data locally since this is on broadway
        Glib::file_set_contents(local_path, Glib::file_get_contents(BROADWAY_DATA_PATH));
        return true;
    };

    json config;
    if (!getSyncConfig(config)) return false;

    std::string server = config["smbserver"];
    std::string user     = config.value("user", "");
    std::string group    = config.value("group", "workgroup");
    std::string password = config.value("password", "");
    std::string share    = config.value("smbshare", "");

    std::vector<std::string> argv = {
        "smbclient", "//" + server + "/" + share,
        "-U", user + "%" + password,
        "-W", group,
        "-c", "get planningData.json " + local_path
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
    auto local_path = Glib::get_user_config_dir() + "/planning/data.json";

    if (isOnBroadway()) { // Save changes locally if on broadway
        Glib::file_set_contents(BROADWAY_DATA_PATH, Glib::file_get_contents(local_path));
        return;
    };

    json config;
    if (!getSyncConfig(config)) return;

    std::string server = config["smbserver"];
    std::string user = config.value("user", "");
    std::string group = config.value("group", "workgroup");
    std::string password = config.value("password", "");
    std::string share = config.value("smbshare", "");

    std::vector<std::string> argv = {
        "smbclient", "//" + server + "/" + share,
        "-U", user + "%" + password,
        "-W", group,
        "-c", "put " + local_path + " planningData.json"
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
