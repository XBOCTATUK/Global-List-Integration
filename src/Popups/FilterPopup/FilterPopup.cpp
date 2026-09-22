#include "FilterPopup.hpp"
#include "../LoadingPopup/LoadingPopup.hpp"
#include "../../Filters/Filters.hpp"
#include "../../Cache/GameLevels/GameLevels.hpp"
#include "../../Cache/Users/Users.hpp"
#include "../../Events/PopulateListEvent.hpp"
#include "../../Events/CloseFiltersEvent.hpp"
#include "../../UI/RangePopup/RangePopup.hpp"

using namespace geode::prelude;

FilterPopup* FilterPopup::create() {
	auto ret = new FilterPopup();
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

constexpr const char* filterWarning =
"Many features in this filter request data from <cy>RobTop servers</c>. The game's servers automatically set usage limits."
"To avoid a temporary ban(up to 1 hour), please avoid updating too frequently."
"Click <cr>I confirm</c> to continue, confirming your understanding of this limitation.";

constexpr const char* filterInfo =
"<cc>The difficulty filter</c> allows you to select levels in the top 50/150/300, Unbounded, or your range.\n"
"<cc>The length filter</c> allows you to select lengths up to 30/60/120/>120, or your range.\n"
"The <cg>rated</c> and <cr>unrated</c> <cc>checkboxes</c> allow you to display only the corresponding levels.\n"
"The <cg>Completed</c> <cc>checkbox</c> only has an effect if the <cf>Username</c> field is not empty and displays the user's completions.\n"
"The <cd>Creator</c> <cc>checkbox</c> only has an effect if the <cf>Creator</c> field is not empty and displays levels from this creator.";

bool FilterPopup::init() {
    if (!Popup::init(425.0f, 260.0f)) return false;

    setID("filter-menu");
    setZOrder(100);
    setTitle("Search filters");

    m_filterContainer = CCNode::create();
    m_filterContainer->setLayout(
        ColumnLayout::create()
        ->setAxisReverse(true)
        ->setGap(10.0f)
    );
    m_filterContainer->setContentSize({ 365.0f, 168.0f });
    m_filterContainer->setAnchorPoint({ 0.5f, 1.0f });
    m_filterContainer->setPosition({ m_mainLayer->getContentWidth() / 2.0f, m_mainLayer->getContentHeight() - 40.0f });
    m_filterContainer->setID("filter-container");
    m_mainLayer->addChild(m_filterContainer);

    m_diffFilterBar = TailyUI::OptionBar::create(
        "GJ_demonIcon_001.png", 0.36f, {"Top 75", "Top 150", "Top 300", "Unbounded", "Custom"},
        [this](const std::string& choice, bool enable) {
            DifficultyFilter diffFilter;
            
            if (choice == "Top 75") diffFilter = DifficultyFilter::Top75;
            if (choice == "Top 150") diffFilter = DifficultyFilter::Top150;
            if (choice == "Top 300") diffFilter = DifficultyFilter::Top300;
            if (choice == "Unbounded") diffFilter = DifficultyFilter::Unbounded;
            if (choice == "Custom") diffFilter = DifficultyFilter::Custom;

            GDL::Filters::setDifficulty(enable ? diffFilter : DifficultyFilter::None);
        },
        []() {
            auto rangePopup = TailyUI::RangePopup::create(
                "Custom difficulty range", 1, 9999,
                [](int from, int to) {
                    GDL::Filters::setCustomDifficulty(from, to);
                }
            );
            rangePopup->show();
        }
    );
    m_diffFilterBar->activateChoice(
        GDL::Filters::getDifficulty(false) == DifficultyFilter::Top75 ?
        "Top 75"
        : GDL::Filters::getDifficulty(false) == DifficultyFilter::Top150 ?
        "Top 150"
        : GDL::Filters::getDifficulty(false) == DifficultyFilter::Top300 ?
        "Top 300"
        : GDL::Filters::getDifficulty(false) == DifficultyFilter::Unbounded ?
        "Unbounded"
        : GDL::Filters::getDifficulty(false) == DifficultyFilter::Custom ?
        "Custom"
        : ""
    );
    m_diffFilterBar->setID("difficulty-filter-bar");
    m_filterContainer->addChild(m_diffFilterBar);

    m_lengthFilterBar = TailyUI::OptionBar::create(
        "GJ_timeIcon_001.png", 0.42f, {"Short", "Medium", "Long", "XL", "Custom"},
        [this](const std::string& choice, bool enable) {
            LengthFilter lengthFilter;
            
            if (choice == "Short") lengthFilter = LengthFilter::Short;
            if (choice == "Medium") lengthFilter = LengthFilter::Medium;
            if (choice == "Long") lengthFilter = LengthFilter::Long;
            if (choice == "XL") lengthFilter = LengthFilter::XL;
            if (choice == "Custom") lengthFilter = LengthFilter::Custom;

            GDL::Filters::setLength(enable ? lengthFilter : LengthFilter::None);
        },
        []() {
            auto rangePopup = TailyUI::RangePopup::create(
                "Custom length range", 1, 9999,
                [](int from, int to) {
                    GDL::Filters::setCustomDifficulty(from, to);
                }
            );
            rangePopup->show();
        }
    );
    m_lengthFilterBar->activateChoice(
        GDL::Filters::getLength(false) == LengthFilter::Short ?
        "Short"
        : GDL::Filters::getLength(false) == LengthFilter::Medium ?
        "Medium"
        : GDL::Filters::getLength(false) == LengthFilter::Long ?
        "Long"
        : GDL::Filters::getLength(false) == LengthFilter::XL ?
        "XL"
        : GDL::Filters::getLength(false) == LengthFilter::Custom ?
        "Custom"
        : ""
    );
    m_lengthFilterBar->setID("length-filter-bar");
    m_filterContainer->addChild(m_lengthFilterBar);


    m_togglerContainer = CCNode::create();
    m_togglerContainer->setContentSize({ m_filterContainer->getContentWidth(), m_filterContainer->getContentHeight() - (m_diffFilterBar->getContentHeight() + 10.0f) * 2.0f });
    m_togglerContainer->setZOrder(15);
    m_togglerContainer->setID("toggler-container");
    m_filterContainer->addChild(m_togglerContainer);

    auto togglerBG = CCScale9Sprite::create("square02b_001.png");
    togglerBG->setColor({ 123, 68, 41 });
    togglerBG->setContentSize(m_togglerContainer->getContentSize());
    togglerBG->setPosition(m_togglerContainer->getContentSize() / 2.0f);
    togglerBG->setID("toggler-background");
    m_togglerContainer->addChild(togglerBG);

    auto togglerContainerContent = CCNode::create();
    togglerContainerContent->setAnchorPoint({ 0.5f, 0.5f });
    togglerContainerContent->setContentSize(m_togglerContainer->getContentSize());
    togglerContainerContent->setPosition(m_togglerContainer->getContentSize() / 2.0f);
    togglerContainerContent->ignoreAnchorPointForPosition(false);
    togglerContainerContent->setID("toggler-container-content");
    m_togglerContainer->addChild(togglerContainerContent);


    auto rateTogglerMenu = CCNode::create();
    rateTogglerMenu->setLayout(
        ColumnLayout::create()
        ->setAxisAlignment(AxisAlignment::End)
        ->setCrossAxisLineAlignment(AxisAlignment::Start)
        ->setPadding({ 10.0f, 10.0f, 10.0f, 10.0f })
        ->setAxisReverse(true)
        ->setGap(10.0f)
    );
    rateTogglerMenu->setAnchorPoint({ 0.5f, 1.0f });
    rateTogglerMenu->setContentSize({togglerContainerContent->getContentWidth() / 3.0f, togglerContainerContent->getContentHeight()});
    rateTogglerMenu->setPosition({togglerContainerContent->getContentWidth() / 6.0f, togglerContainerContent->getContentHeight()});
    rateTogglerMenu->ignoreAnchorPointForPosition(false);
    rateTogglerMenu->setID("rate-toggler-menu");
    togglerContainerContent->addChild(rateTogglerMenu);

    m_ratedToggler = TailyUI::LabeledCheckbox::create(
        "Rated", 24.0f, "If enabled, the Demonlist will display only rated levels.",
        [this](bool enable) {
            GDL::Filters::setRated(enable);
            if (enable) {
                GDL::Filters::setUnrated(false);
                m_unratedToggler->setToggled(false);
            }
        }
    );
    m_ratedToggler->setToggled(GDL::Filters::getRated(false));
    m_ratedToggler->setID("rated-toggler");
    rateTogglerMenu->addChild(m_ratedToggler);

    m_unratedToggler = TailyUI::LabeledCheckbox::create(
        "Unrated", 24.0f, "If enabled, the Demonlist will display only unrated levels.",
        [this](bool enable) {
            GDL::Filters::setUnrated(enable);
            if (enable) {
                GDL::Filters::setRated(false);
                m_ratedToggler->setToggled(false);
            }
        }
    );
    m_unratedToggler->setToggled(GDL::Filters::getUnrated(false));
    m_unratedToggler->setID("unrated-toggler");
    rateTogglerMenu->addChild(m_unratedToggler);


    auto userTogglerMenu = CCNode::create();
    userTogglerMenu->setLayout(
        ColumnLayout::create()
        ->setAxisAlignment(AxisAlignment::End)
        ->setCrossAxisLineAlignment(AxisAlignment::Start)
        ->setPadding({ 10.0f, 10.0f, 10.0f, 10.0f })
        ->setAxisReverse(true)
        ->setGap(10.0f)
    );
    userTogglerMenu->setAnchorPoint({ 0.5f, 1.0f });
    userTogglerMenu->setContentSize({togglerContainerContent->getContentWidth() / 3.0f, togglerContainerContent->getContentHeight()});
    userTogglerMenu->setPosition({togglerContainerContent->getContentWidth() / 2.0f, togglerContainerContent->getContentHeight()});
    userTogglerMenu->ignoreAnchorPointForPosition(false);
    userTogglerMenu->setID("user-toggler-menu");
    togglerContainerContent->addChild(userTogglerMenu);

    m_completedToggler = TailyUI::LabeledCheckbox::create(
        "Completed By", 24.0f, "If enabled, the Demonlist will display only completions by the selected Global Demonlist user.",
        [this](bool enable) {
            GDL::Filters::setCompletedBy(enable);
        }
    );
    m_completedToggler->setToggled(GDL::Filters::getCompletedBy(false));
    m_completedToggler->setID("completed-by-toggler"); // Toggler became the top 1 player
    userTogglerMenu->addChild(m_completedToggler);

    m_createdByToggler = TailyUI::LabeledCheckbox::create(
        "Created By", 24.0f, "If enabled, the Demonlist will display only completions by the selected creator.",
        [this](bool enable) {
            GDL::Filters::setCreatedBy(enable);
        }
    );
    m_createdByToggler->setToggled(GDL::Filters::getCreatedBy(false));
    m_createdByToggler->setID("created-by-toggler"); // Toggler became the top 1 creator
    userTogglerMenu->addChild(m_createdByToggler);


    auto inputMenu = CCNode::create();
    inputMenu->setLayout(
        ColumnLayout::create()
        ->setAxisAlignment(AxisAlignment::End)
        ->setPadding({ 10.0f, 10.0f, 10.0f, 10.0f })
        ->setAxisReverse(true)
        ->setAutoScale(false)
        ->setGap(10.0f)
    );
    inputMenu->setAnchorPoint({ 0.5f, 1.0f });
    inputMenu->setContentSize({togglerContainerContent->getContentWidth() / 3.0f, togglerContainerContent->getContentHeight()});
    inputMenu->setPosition({togglerContainerContent->getContentWidth() / 6.0f * 5.0f, togglerContainerContent->getContentHeight()});
    inputMenu->ignoreAnchorPointForPosition(false);
    inputMenu->setID("input-menu");
    togglerContainerContent->addChild(inputMenu);

    m_usernameInput = TextInput::create(120.0f, "Username");
	m_usernameInput->setScale(0.8f);
	m_usernameInput->setMaxCharCount(32);
	m_usernameInput->setString(Mod::get()->getSavedValue<std::string>("username", ""));
	m_usernameInput->setCallback([this](std::string text) {
		Mod::get()->setSavedValue<std::string>("username", text);
	});
    m_usernameInput->setID("username-input");
    inputMenu->addChild(m_usernameInput);

    m_creatorNameInput = TextInput::create(120.0f, "Creator Name");
	m_creatorNameInput->setScale(0.8f);
	m_creatorNameInput->setMaxCharCount(32);
	m_creatorNameInput->setString(Mod::get()->getSavedValue<std::string>("creatorName", ""));
	m_creatorNameInput->setCallback([this](std::string text) {
		Mod::get()->setSavedValue<std::string>("creatorName", text);
	});
    m_creatorNameInput->setID("creator-name-input");
    inputMenu->addChild(m_creatorNameInput);


    m_filterContainer->updateLayout();
    rateTogglerMenu->updateLayout();
    userTogglerMenu->updateLayout();
    inputMenu->updateLayout();
    
    auto applySpr = ButtonSprite::create("Apply", 0.8f);
    auto applyBtn = CCMenuItemExt::createSpriteExtra(
        applySpr, [this](auto) {
            GDL::Filters::setUsername(m_usernameInput->getString());
            GDL::Filters::setCreatorName(m_creatorNameInput->getString());

            int cachedUserID = GDL::Cache::Users::getUserIDByUsername(GDL::Filters::getUsername(false));
            
            bool isOutOfDate = GDL::Cache::GameLevels::isOutToDate();
            auto cachedUser = GDL::Cache::Users::getUser(cachedUserID);

            bool needLevelData = GDL::Filters::isLevelDataRequired() && isOutOfDate;
            bool needUserData = GDL::Filters::getCompletedBy(false) && !cachedUser;

            if (!needLevelData && !needUserData) {
                PopulateListEvent().send();
                
                onClose(nullptr);
                return;
            }
            
            if (!Mod::get()->getSavedValue<bool>("showWarning", true)) {
                LoadingPopup::create()->show();
            }
            else {
                createQuickPopup("Warning", filterWarning, "Oh, no", "I confirm", [](auto, bool confirmBtn) {
                    if (confirmBtn) {
                        Mod::get()->setSavedValue<bool>("showWarning", false);
                        LoadingPopup::create()->show();
                    }
                });
            }
        }
    );
	applyBtn->setPosition({ m_buttonMenu->getContentWidth() / 2.0f, 28.0f });
    applyBtn->setID("apply-button");
	m_buttonMenu->addChild(applyBtn);

    m_closeListener = CloseFiltersEvent().listen(
		[this]() { Popup::onClose(nullptr); }
	);

    return true;
}