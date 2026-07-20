#include "LabeledCheckbox.hpp"

namespace TailyUI {
    LabeledCheckbox* LabeledCheckbox::create(
        const char* name, float height, const char* info,
        Function<void(bool)> togglerCallback
    ) {
    	auto ret = new LabeledCheckbox();
        if (ret && ret->init(name, height, info, std::move(togglerCallback))) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool LabeledCheckbox::init(
        const char* name, float height, const char* info,
        Function<void(bool)> togglerCallback
    ) {
        if (!CCMenu::init()) return false;

        // setTouchEnabled(true);
        setTogglerCallback(std::move(togglerCallback));

        m_toggler = CCMenuItemExt::createTogglerWithStandardSprites(1.0f,
            [this](CCMenuItemToggler* self) {
                bool enabled = !self->isOn();

                if (m_togglerCallback) {
                    m_togglerCallback(enabled);
                }
            }
        );
        m_toggler->setScale(height / m_toggler->getContentHeight());
        m_toggler->setPosition({ m_toggler->getScaledContentWidth() / 2.0f, height / 2.0f });
        addChild(m_toggler);

        m_label = CCLabelBMFont::create(name, "bigFont.fnt");
        m_label->limitLabelWidth( 80.0f, 0.4f, 0.3f );
        m_label->setAnchorPoint({ 0.0f, 0.5f });
        m_label->setPosition({ m_toggler->getScaledContentWidth() + 5.0f, height / 2.0f });
        addChild(m_label);

        setContentSize({m_label->getPositionX() + m_label->getScaledContentWidth(), height});
        setAnchorPoint({ 0.5f, 0.5f });

        if (info) {
            m_infoBtn = InfoAlertButton::create(fmt::format("'{}' toggler", name), info, 0.5f);
            m_infoBtn->setPosition(getContentSize() - ccp(-3.0f, m_infoBtn->getScaledContentHeight() / 2.0f));
            addChild(m_infoBtn);
        }

        return true;
    }

    void LabeledCheckbox::setToggled(bool enable) {
        m_toggler->toggle(enable);
    }

    void LabeledCheckbox::setTogglerCallback(Function<void(bool)> togglerCallback) {
        m_togglerCallback = std::move(togglerCallback);
    }
};