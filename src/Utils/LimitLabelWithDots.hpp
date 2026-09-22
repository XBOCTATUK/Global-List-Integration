namespace Utils {
    inline void limitLabelWithDots(cocos2d::CCLabelBMFont* label, float width) {
        if (label->getScaledContentWidth() <= width) return;

        std::string text = label->getString();
        for (size_t i = text.size()-1; i > 0; i--) {
            auto subString = geode::utils::string::trim(text.substr(0, i));
            
            label->setString(fmt::format("{}...", subString).c_str());
            if (label->getScaledContentWidth() <= width) return;
        }
    }
}