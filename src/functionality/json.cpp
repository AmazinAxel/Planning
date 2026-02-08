#include <glibmm.h>
#include <giomm.h> // Gio file stuff
#include <nlohmann/json.hpp>
using json = nlohmann::json;

json initLoadJSON() {
    auto config_dir = Glib::get_user_config_dir() + "/planning";
    auto data_path  = config_dir + "/data.json";

    auto dir_file = Gio::File::create_for_path(config_dir);
    if (!dir_file->query_exists())
        dir_file->make_directory_with_parents();

    // Create data file if nonexistent
    auto data_file = Gio::File::create_for_path(data_path);
    if (!data_file->query_exists()) {
        json newFileData = {{"plans", json::array()}};
        Glib::file_set_contents(data_path, newFileData.dump(4));
        return newFileData;
    };

    std::string contents = Glib::file_get_contents(data_path);
    return json::parse(contents);
};

void saveJSON(const json& json) {
    auto path = Glib::get_user_config_dir() + "/planning/data.json";
    Glib::file_set_contents(path, json.dump(4));
};
