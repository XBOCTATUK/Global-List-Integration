#include <Geode/cocos/cocoa/CCGeometry.h>

namespace Utils {
    inline float calculateCoverScale(const CCSize& targetSize, const CCSize& spriteSize) {
        if (targetSize.width / targetSize.height <= spriteSize.width / spriteSize.height) {
            return targetSize.height / spriteSize.height;
        }
        return targetSize.width / spriteSize.width; 
    }
};