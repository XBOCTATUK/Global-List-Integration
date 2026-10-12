#include "Settings.hpp"

using namespace geode::prelude;

namespace Settings {
    static bool g_loadPlacementOnlyOnDemonlist = Mod::get()->getSettingValue<bool>("load-only-on-demonlist");
    static bool g_shouldLoadPlacement = false;
    static bool g_increaseLevelsPerPage = Mod::get()->getSettingValue<bool>("increase-levels-per-page");
    static bool g_increaseCellsPerPage = Mod::get()->getSettingValue<bool>("increase-cells-per-page");
    static bool g_compactCells = Mod::get()->getSettingValue<bool>("compact-cells");

    bool shouldLoadPlacement() {
        return g_loadPlacementOnlyOnDemonlist ? g_shouldLoadPlacement : true;
    }

    void setShouldLoadPlacement(bool enable) {
        g_shouldLoadPlacement = enable;
    }

    int demonlistPageSize() {
        return g_increaseLevelsPerPage ? 25 : 10;
    }

    int leaderboardPageSize() {
        return g_increaseCellsPerPage ? 50 : 25;
    }

    bool compactLeaderboardCells() {
        return g_compactCells;
    }
};

$on_mod(Loaded) {
    listenForSettingChanges<bool>("load-only-on-demonlist", [](bool value) {
        Settings::g_loadPlacementOnlyOnDemonlist = value;
    });

    listenForSettingChanges<bool>("increase-levels-per-page", [](bool value) {
        Settings::g_increaseLevelsPerPage = value;
    });

    listenForSettingChanges<bool>("increase-cells-per-page", [](bool value) {
        Settings::g_increaseCellsPerPage = value;
    });

    listenForSettingChanges<bool>("compact-cells", [](bool value) {
        Settings::g_compactCells = value;
    });
}