#include "OptionBar.hpp"

namespace TailyUI {
    OptionBar* OptionBar::create(
        const char* icon, float labelScale, 
        const std::initializer_list<std::string>& values,
        Function<void(std::string, bool)> choiceCallback,
        Function<void()> optionsCallback
    ) {
    	auto ret = new OptionBar();
        if (ret && ret->init(icon, labelScale, values, std::move(choiceCallback), std::move(optionsCallback))) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool OptionBar::init(
        const char* icon, float labelScale, 
        const std::initializer_list<std::string>& values,
        Function<void(std::string, bool)> choiceCallback,
        Function<void()> optionsCallback
    ) {
        if (!CCNode::init()) return false;

        setContentSize({ 365.0f, 35.0f });
        setAnchorPoint({ 0.5f, 0.5f });
        setZOrder(10);

        setChoiceCallback(std::move(choiceCallback));
        setOptionsCallback(std::move(optionsCallback));

        auto bg = CCScale9Sprite::create("square02b_001.png");
        bg->setColor({ 123, 68, 41 });
        bg->setContentSize(getContentSize());
        bg->setPosition(getContentSize() / 2.0f);
        addChild(bg);

        m_menu = CCMenu::create();
        m_menu->setLayout(
            RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Between)
            ->setCrossAxisOverflow(false)
            ->setPadding({ 10.0f, 0.0f, 10.0f, 0.0f })
            ->setAutoScale(false)
        );
        m_menu->setContentSize(getContentSize());
        m_menu->setPosition(getContentSize() / 2.0f);
        addChild(m_menu);

        m_icon = CCSprite::createWithSpriteFrameName(icon);
        m_icon->setScale(23.0f / m_icon->getContentHeight());
        m_menu->addChild(m_icon);

        for (auto& value : values) {
            auto choiceLabel = CCLabelBMFont::create(value.c_str(), "bigFont.fnt");
            choiceLabel->setScale(labelScale);
            choiceLabel->setColor({ 125, 125, 125 });

            auto choiceBtn = CCMenuItemExt::createSpriteExtra(choiceLabel, [this](CCMenuItemSpriteExtra* self) {
                if (m_selectedBtn) {
                    auto label = static_cast<CCLabelBMFont*>(m_selectedBtn->getNormalImage());
                    label->setColor({ 125, 125, 125 });

                    if (m_selectedBtn == self) {
                        m_selectedBtn = nullptr;
                        m_choiceCallback(label->getString(), false);
                        return;
                    }
                }
                
                auto normalSpr = self->getNormalImage();
                auto label = static_cast<CCLabelBMFont*>(normalSpr);

                label->setColor({ 255, 255, 255 });
                m_selectedBtn = self;

                if (m_choiceCallback) {
                    m_choiceCallback(label->getString(), true);
                }
            });

            m_menu->addChild(choiceBtn);
            m_choices[value] = choiceBtn;
        }

        auto optionsSpr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
        optionsSpr->setScale(23.0f / optionsSpr->getContentHeight());
        auto optionBtn = CCMenuItemExt::createSpriteExtra(optionsSpr, [this](auto) {
            if (m_optionsCallback) {
                m_optionsCallback();
            }
        });
        m_menu->addChild(optionBtn);

        m_menu->updateLayout();

        return true;
    }

    void OptionBar::activateChoice(std::string choiceName) {
        if (choiceName == "") return;

        auto it = m_choices.find(choiceName);
        if (it == m_choices.end()) return;
        
        m_choices[choiceName]->activate();
    }

    void OptionBar::setChoiceCallback(Function<void(std::string, bool)> callback) {
        m_choiceCallback = std::move(callback);
    }

    void OptionBar::setOptionsCallback(Function<void()> callback) {
        m_optionsCallback = std::move(callback);
    }
};