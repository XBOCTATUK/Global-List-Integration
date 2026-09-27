#include <Geode/DefaultInclude.hpp>
#include "API/Levels/Levels.hpp"
#include "Cache/Levels/Levels.hpp"

class LevelDataUpdater : public cocos2d::CCObject {
public:
    void updateData(float dt) {
        GDL::Cache::Levels::clear();
        GDL::API::Levels::getDemonlist();
    }
};

$on_game(Loaded) {
    auto levelDataUpdater = new LevelDataUpdater{};

    levelDataUpdater->updateData(0.0f);
    cocos2d::CCDirector::get()->getScheduler()->scheduleSelector(
        schedule_selector(LevelDataUpdater::updateData),
        levelDataUpdater, 1800.0f, false
    );

    levelDataUpdater->release();
}