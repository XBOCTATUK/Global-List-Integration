#include "Settings.hpp"

namespace Settings {
    static bool g_loadPlacementOnlyOnDemonlist = Mod::get()->getSettingValue<bool>("load-only-on-demonlist");
    static bool g_shouldLoadPlacement = false;
    static bool g_increaseLevelsPerPage = Mod::get()->getSettingValue<bool>("increase-levels-per-page");

    bool shouldLoadPlacement() {
        return g_loadPlacementOnlyOnDemonlist ? g_shouldLoadPlacement : true;
    }

    void setShouldLoadPlacement(bool enable) {
        g_shouldLoadPlacement = enable;
    }

    bool increaseLevelsPerPage() {
        return g_increaseLevelsPerPage;
    }
};

$on_mod(Loaded) {
    listenForSettingChanges<bool>("load-only-on-demonlist", [](bool value) {
        Settings::g_loadPlacementOnlyOnDemonlist = value;
    });

    listenForSettingChanges<bool>("increase-levels-per-page", [](bool value) {
        Settings::g_increaseLevelsPerPage = value;
    });
}