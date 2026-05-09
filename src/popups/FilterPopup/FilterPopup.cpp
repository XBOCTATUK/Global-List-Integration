#include "./FilterPopup.hpp"

FilterPopup* FilterPopup::create() {
	auto ret = new FilterPopup();
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	delete ret;
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
"The <cg>Completed</c> <cc>checkbox</c> only has an effect if the <cf>Username</c> field is not empty and displays the player's completions.\n"
"The <cd>Creator</c> <cc>checkbox</c> only has an effect if the <cf>Creator</c> field is not empty and displays levels from this creator.";

bool FilterPopup::init() {
	if (!Popup::init(450.0f, 280.0f)) return false;

	this->setID("filter-menu");
	this->setZOrder(100);
	this->setTitle("Search filters");

	auto& displayFilters = GlobalList::Filters::getDisplayFilters();

	auto optionsSpr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
	optionsSpr->setContentSize({ 50.5f, 50.5f });
	optionsSpr->setScale(0.46);


	auto diffFilterBG = CCScale9Sprite::create("square02b_001.png");
	diffFilterBG->setColor({ 123, 68, 41 });
	diffFilterBG->setContentSize({ 365.0f, 35.0f });
	diffFilterBG->setPosition({ 225.0f, 225.0f });
	m_mainLayer->addChild(diffFilterBG);

	auto diffFilterMenu = CCMenu::create();
	diffFilterMenu->setLayout(RowLayout::create()
		->setGap(16.0f)
		->setAxisAlignment(AxisAlignment::Between)
		->setAutoScale(false));
	diffFilterMenu->setContentSize({ 340.0f, 35.0f });
	diffFilterMenu->setPosition({ 225.0f, 225.0f });
	diffFilterMenu->ignoreAnchorPointForPosition(false);
	m_mainLayer->addChild(diffFilterMenu);

	auto demonSpr = CCSprite::createWithSpriteFrameName("GJ_demonIcon_001.png");
	demonSpr->setContentSize({ 26.5f, 26.5f });
	demonSpr->setScale(0.90f);
	diffFilterMenu->addChild(demonSpr);

	auto top50Label = CCLabelBMFont::create("Top 50", "bigFont.fnt");
	top50Label->setScale(0.36f);
	if (displayFilters.diffFilter[0]) top50Label->setColor({ 255, 255, 255 });
	else top50Label->setColor({ 125, 125, 125 });
	auto top50Btn = CCMenuItemExt::createSpriteExtra(top50Label, [this, top50Label](auto) {
		changeDiffFilter(DifficultyFilter::Top50, top50Label);
		});
	diffFilterMenu->addChild(top50Btn);

	auto top150Label = CCLabelBMFont::create("Top 150", "bigFont.fnt");
	top150Label->setScale(0.36f);
	if (displayFilters.diffFilter[1]) top150Label->setColor({ 255, 255, 255 });
	else top150Label->setColor({ 125, 125, 125 });
	auto top150Btn = CCMenuItemExt::createSpriteExtra(top150Label, [this, top150Label](auto) {
		changeDiffFilter(DifficultyFilter::Top150, top150Label);
		});
	diffFilterMenu->addChild(top150Btn);

	auto top300Label = CCLabelBMFont::create("Top 300", "bigFont.fnt");
	top300Label->setScale(0.36f);
	if (displayFilters.diffFilter[2]) top300Label->setColor({ 255, 255, 255 });
	else top300Label->setColor({ 125, 125, 125 });
	auto top300Btn = CCMenuItemExt::createSpriteExtra(top300Label, [this, top300Label](auto) {
		changeDiffFilter(DifficultyFilter::Top300, top300Label);
		});
	diffFilterMenu->addChild(top300Btn);

	auto unboundedLabel = CCLabelBMFont::create("Unbounded", "bigFont.fnt");
	unboundedLabel->setScale(0.36f);
	if (displayFilters.diffFilter[3]) unboundedLabel->setColor({ 255, 255, 255 });
	else unboundedLabel->setColor({ 125, 125, 125 });
	auto unboundedBtn = CCMenuItemExt::createSpriteExtra(unboundedLabel, [this, unboundedLabel](auto) {
		changeDiffFilter(DifficultyFilter::Unbounded, unboundedLabel);
		});
	diffFilterMenu->addChild(unboundedBtn);

	auto customDiffLabel = CCLabelBMFont::create("Custom", "bigFont.fnt");
	customDiffLabel->setScale(0.36f);
	if (displayFilters.diffFilter[4]) customDiffLabel->setColor({ 255, 255, 255 });
	else customDiffLabel->setColor({ 125, 125, 125 });
	auto customDiffBtn = CCMenuItemExt::createSpriteExtra(customDiffLabel, [this, customDiffLabel](auto) {
		changeDiffFilter(DifficultyFilter::Custom, customDiffLabel);
		});
	diffFilterMenu->addChild(customDiffBtn);

	auto customDiffOptionsBtn = CCMenuItemExt::createSpriteExtra(optionsSpr, [this](auto) { RangePopup::create(FilterType::Difficulty)->show(); });
	customDiffOptionsBtn->updateLayout();
	diffFilterMenu->addChild(customDiffOptionsBtn);

	diffFilterMenu->updateLayout();


	auto lengthFilterBG = CCScale9Sprite::create("square02b_001.png");
	lengthFilterBG->setColor({ 123, 68, 41 });
	lengthFilterBG->setContentSize({ 365.0f, 35.0f });
	lengthFilterBG->setPosition({ 225.0f, 180.0f });
	m_mainLayer->addChild(lengthFilterBG);

	auto lengthFilterMenu = CCMenu::create();
	lengthFilterMenu->setLayout(RowLayout::create()
		->setGap(16.0f)
		->setAxisAlignment(AxisAlignment::Between)
		->setAutoScale(false));
	lengthFilterMenu->setContentSize({ 340.0f, 35.0f });
	lengthFilterMenu->setPosition({ 225.0f, 180.0f });
	lengthFilterMenu->ignoreAnchorPointForPosition(false);
	m_mainLayer->addChild(lengthFilterMenu);

	auto clockSpr = CCSprite::createWithSpriteFrameName("GJ_timeIcon_001.png");
	clockSpr->setContentSize({ 23.0f, 23.0f });
	lengthFilterMenu->addChild(clockSpr);

	auto shortLenLabel = CCLabelBMFont::create("Short", "bigFont.fnt");
	shortLenLabel->setScale(0.42f);
	if (displayFilters.lengthFilter[0]) shortLenLabel->setColor({ 255, 255, 255 });
	else shortLenLabel->setColor({ 125, 125, 125 });
	auto shortLenBtn = CCMenuItemExt::createSpriteExtra(shortLenLabel, ([this, shortLenLabel](auto) {
		changeLengthFilter(LengthFilter::Short, shortLenLabel);
		}));
	lengthFilterMenu->addChild(shortLenBtn);

	auto mediumLenLabel = CCLabelBMFont::create("Medium", "bigFont.fnt");
	mediumLenLabel->setScale(0.42f);
	if (displayFilters.lengthFilter[1]) mediumLenLabel->setColor({ 255, 255, 255 });
	else mediumLenLabel->setColor({ 125, 125, 125 });
	auto mediumLenBtn = CCMenuItemExt::createSpriteExtra(mediumLenLabel, [this, mediumLenLabel](auto) {
		changeLengthFilter(LengthFilter::Medium, mediumLenLabel);
		});
	lengthFilterMenu->addChild(mediumLenBtn);

	auto longLenLabel = CCLabelBMFont::create("Long", "bigFont.fnt");
	longLenLabel->setScale(0.42f);
	if (displayFilters.lengthFilter[2]) longLenLabel->setColor({ 255, 255, 255 });
	else longLenLabel->setColor({ 125, 125, 125 });
	auto longLenBtn = CCMenuItemExt::createSpriteExtra(longLenLabel, [this, longLenLabel](auto) {
		changeLengthFilter(LengthFilter::Long, longLenLabel);
		});
	lengthFilterMenu->addChild(longLenBtn);

	auto xlLenLabel = CCLabelBMFont::create("XL", "bigFont.fnt");
	xlLenLabel->setScale(0.42f);
	if (displayFilters.lengthFilter[3]) xlLenLabel->setColor({ 255, 255, 255 });
	else xlLenLabel->setColor({ 125, 125, 125 });
	auto xlLenBtn = CCMenuItemExt::createSpriteExtra(xlLenLabel, [this, xlLenLabel](auto) {
		changeLengthFilter(LengthFilter::XL, xlLenLabel);
		});
	lengthFilterMenu->addChild(xlLenBtn);

	auto customLenLabel = CCLabelBMFont::create("Custom", "bigFont.fnt");
	customLenLabel->setScale(0.42f);
	if (displayFilters.lengthFilter[4]) customLenLabel->setColor({ 255, 255, 255 });
	else customLenLabel->setColor({ 125, 125, 125 });
	auto customLenBtn = CCMenuItemExt::createSpriteExtra(customLenLabel, [this, customLenLabel](auto) {
		changeLengthFilter(LengthFilter::Custom, customLenLabel);
		});
	lengthFilterMenu->addChild(customLenBtn);

	auto customLenOptionsBtn = CCMenuItemExt::createSpriteExtra(optionsSpr, [this](auto) { RangePopup::create(FilterType::Lenght)->show(); });
	customLenOptionsBtn->updateLayout();
	lengthFilterMenu->addChild(customLenOptionsBtn);

	lengthFilterMenu->updateLayout();


	auto otherFiltersBG = CCScale9Sprite::create("square02b_001.png");
	otherFiltersBG->setColor({ 123, 68, 41 });
	otherFiltersBG->setContentSize({ 365.0f, 100.0f });
	otherFiltersBG->setPosition({ 225.0f, 102.5f });
	m_mainLayer->addChild(otherFiltersBG);

	auto otherFiltersMenu = CCMenu::create();
	otherFiltersMenu->setContentSize({ 365.0f, 100.0f });
	otherFiltersMenu->setPosition({ 225.0f, 102.5f });
	otherFiltersMenu->ignoreAnchorPointForPosition(false);
	m_mainLayer->addChild(otherFiltersMenu);

	m_ratedFilterToggler = CCMenuItemExt::createTogglerWithStandardSprites(1.0f, [this](CCMenuItemToggler* toggler) {
		if (!toggler->isOn()) {
			GlobalList::Filters::setRated(true);
			m_unratedFilterToggler->toggle(false);
		}
		else GlobalList::Filters::setRated(false);
	});
	m_ratedFilterToggler->toggle(displayFilters.rated);
	m_ratedFilterToggler->setScale(0.75f);
	m_ratedFilterToggler->setPosition({ 19.0f, 81.0f });
	otherFiltersMenu->addChild(m_ratedFilterToggler);

	auto ratedFilterLabel = CCLabelBMFont::create("Rated", "bigFont.fnt");
	ratedFilterLabel->setAnchorPoint({ 0.0f, 0.5f });
	ratedFilterLabel->setScale(0.4f);
	ratedFilterLabel->setPosition({ 36.0f, 81.0f });
	otherFiltersMenu->addChild(ratedFilterLabel);

	m_unratedFilterToggler = CCMenuItemExt::createTogglerWithStandardSprites(1.0f, [this](CCMenuItemToggler* toggler) {
		if (!toggler->isOn()) {
			GlobalList::Filters::setUnrated(true);
			m_ratedFilterToggler->toggle(false);
		}
		else GlobalList::Filters::setUnrated(false);
	});
	m_unratedFilterToggler->toggle(displayFilters.unrated);
	m_unratedFilterToggler->setScale(0.75f);
	m_unratedFilterToggler->setPosition({ 19.0f, 50.0f });
	otherFiltersMenu->addChild(m_unratedFilterToggler);

	auto unratedFilterLabel = CCLabelBMFont::create("Unrated", "bigFont.fnt");
	unratedFilterLabel->setAnchorPoint({ 0.0f, 0.5f });
	unratedFilterLabel->setScale(0.4f);
	unratedFilterLabel->setPosition({ 36.0f, 50.0f });
	otherFiltersMenu->addChild(unratedFilterLabel);

	auto completedFilterToggler = CCMenuItemExt::createTogglerWithStandardSprites(1.0f, [this](CCMenuItemToggler* toggler) {
		if (!toggler->isOn()) GlobalList::Filters::setCompleted(true);
		else GlobalList::Filters::setCompleted(true);
	});
	completedFilterToggler->toggle(displayFilters.completed);
	completedFilterToggler->setScale(0.75f);
	completedFilterToggler->setPosition({ 143.5f, 81.0f });
	otherFiltersMenu->addChild(completedFilterToggler);

	auto completedFilterLabel = CCLabelBMFont::create("Completed", "bigFont.fnt");
	completedFilterLabel->setAnchorPoint({ 0.0f, 0.5f });
	completedFilterLabel->setScale(0.4f);
	completedFilterLabel->setPosition({ 160.5f, 81.0f });
	otherFiltersMenu->addChild(completedFilterLabel);

	auto creatorFilterToggler = CCMenuItemExt::createTogglerWithStandardSprites(1.0f, [this](CCMenuItemToggler* toggler) {
		if (!toggler->isOn()) GlobalList::Filters::setByHolder(true);
		else GlobalList::Filters::setByHolder(false);
	});
	creatorFilterToggler->toggle(displayFilters.byHolder);
	creatorFilterToggler->setScale(0.75f);
	creatorFilterToggler->setPosition({ 143.5f, 50.0f });
	otherFiltersMenu->addChild(creatorFilterToggler);

	auto creatorFilterLabel = CCLabelBMFont::create("Creator", "bigFont.fnt");
	creatorFilterLabel->setAnchorPoint({ 0.0f, 0.5f });
	creatorFilterLabel->setScale(0.4f);
	creatorFilterLabel->setPosition({ 160.5f, 50.0f });
	otherFiltersMenu->addChild(creatorFilterLabel);

	auto usernameInput = TextInput::create(106.0f / 0.9f, "Username");
	usernameInput->setAnchorPoint({ 0.0f, 0.5f });
	usernameInput->setScale(0.9f);
	usernameInput->setPosition({ 254.0f, 81.0f });
	usernameInput->setMaxCharCount(32);
	usernameInput->setString(Mod::get()->getSavedValue<std::string>("username", ""));
	usernameInput->setCallback([this](const std::string& text) {
		Mod::get()->setSavedValue<std::string>("username", text);
	});
	otherFiltersMenu->addChild(usernameInput);

	auto holderInput = TextInput::create(106.0f / 0.9f, "Holder");
	holderInput->setAnchorPoint({ 0.0f, 0.5f });
	holderInput->setScale(0.9f);
	holderInput->setPosition({ 254.0f, 50.0f });
	holderInput->setMaxCharCount(32);
	holderInput->setString(Mod::get()->getSavedValue<std::string>("holder", ""));
	holderInput->setCallback([this](const std::string& text) {
		Mod::get()->setSavedValue<std::string>("holder", text);
	});
	otherFiltersMenu->addChild(holderInput);
	
	auto applyBtnSpr = ButtonSprite::create("Apply");
	auto applyBtn = CCMenuItemExt::createSpriteExtra(applyBtnSpr, [this, usernameInput, holderInput](auto) {
		auto time = std::chrono::high_resolution_clock::now();
		auto seconds = std::chrono::duration_cast<std::chrono::seconds>( time.time_since_epoch() );
		int64_t timestamp = seconds.count();

		int lastUpdate = Mod::get()->getSavedValue<int64_t>("lastLevelDataUpdate", 0);
		int difference = timestamp - lastUpdate;

		auto& levelFilters = GlobalList::Filters::getDisplayFilters();
		if (!levelFilters.isDataRequired() || (GlobalList::Cache::levelDataSize() == 0 ? false : difference < 900)) {
			GlobalList::Filters::applyFilters();
			PopulateListEvent().send();
			
			onClose(this);
			return;
		}

		GlobalList::Filters::setUsername(usernameInput->getString());
		GlobalList::Filters::setHolderName(holderInput->getString());
		
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

	});
	applyBtn->setPosition({ 225.0f, 28.0f });
	m_buttonMenu->addChild(applyBtn);

	auto infoBtn = InfoAlertButton::create("How to use filters", filterInfo, 1.0f);
	infoBtn->setPosition({24.0f, 24.0f});
	m_buttonMenu->addChild(infoBtn);

	m_closeListener = MyCloseEvent(PopupType::FilterPopup).listen(
		[this]() { Popup::onClose(this); }
	);

	return true;
}

void FilterPopup::changeLengthFilter(LengthFilter type, CCLabelBMFont* label) {
	auto& displayFilters = GlobalList::Filters::getDisplayFilters();
	bool lengthFilter = displayFilters.lengthFilter[(int)type];
	if (!lengthFilter) {
		label->setColor({ 255, 255, 255 });
		GlobalList::Filters::setLengthFilter(type, true);
	}
	else {
		label->setColor({ 125, 125, 125 });
		GlobalList::Filters::setLengthFilter(type, false);
	}
}

void FilterPopup::changeDiffFilter(DifficultyFilter type, CCLabelBMFont* label) {
	auto& displayFilters = GlobalList::Filters::getDisplayFilters();
	bool diffFilter = displayFilters.diffFilter[(int)type];
	if (!diffFilter) {
		label->setColor({ 255, 255, 255 });
		GlobalList::Filters::setDifficultyFilter(type, true);
	}
	else {
		label->setColor({ 125, 125, 125 });
		GlobalList::Filters::setDifficultyFilter(type, false);
	}
}