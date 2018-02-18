#include "EffectsTestState.h"
#include "SortItemModel.h"
#include "Window.h"
#include "ETScrollView.h"
#include "TextMenuTable.h"

#include "StateManager.h"

#include "MasterDataManager.h"
#include "MasterEffectModel.h"
#include "Converter.h"
#include "CommonEffect.h"
#include "MessageButton.h"
#include "DefaultLabel.h"
#include "CommonButton.h"

USING_NS_CC;
USING_NS_ENGTRAIN;

bool EffectsTestState::init()
{
    if (!State::initView(true)) {
        return false;
    }
    
    std::vector<SortItemModel_ptr> itemModels;

    auto list = MasterDataManager::getInstance()->findAll(MasterDataManager::TYPE_EFFECT, "id");
    for (auto effectId : list) {
        auto model = MasterEffectModel::create(Converter::toInt(effectId));
        itemModels.push_back(SortItemModel::create(StringUtils::format("Id: %d", model->getId()), model->getId() + effectRef));
    }

    auto defaultEffect = MasterEffectModel::create(Converter::toInt(list[0]));
    activeEffectId = defaultEffect->getId();

    auto window = Window::createFullScreen();
    window->setPosition(320, 455);
    this->addChild(window);
    
    window->addCloseButtonWithBorder(Event::CLOSE);
    window->setTextHeader("Effects");

    auto left = MessageButton::create("-", Event::DURATION_DOWN, false);
    left->setPosition(640 - 250 - 130 , 870);
    left->setGlobal(false);
    this->addChild(left);

    auto active = DefaultLabel::create(StringUtils::format("Id %d", defaultEffect->getId()), 30.0f);
    active->setPosition(190, 870);
    active->setTag(4);
    this->addChild(active);

    this->currentDuration = defaultEffect->getDuration();
    auto durLabel = DefaultLabel::create(StringUtils::format("Duration:%.2f", this->currentDuration), 30.0f);
    durLabel->setPosition(640 - 250, 870);
    durLabel->setTag(2);
    this->addChild(durLabel);

    auto right = MessageButton::create("+", Event::DURATION_UP, false);
    right->setPosition(640 - 250 + 130 , 870);
    right->setGlobal(false);
    this->addChild(right);

    auto playButton = CommonButton::create("navi_finger.png", Event::PLAY);
    playButton->setGlobal(false);
    playButton->setTag(3);
    playButton->setScale(0.4, 0.4);
    playButton->setPosition(Point(580, 870));
    this->addChild(playButton);

    auto scroll = engTrain::FastScrollView::create();
    scroll->setAnchorPoint(Point(0, 0));
    scroll->setPosition(Point(1, 96));
    scroll->setContentSize(Size(Window::DEFAULT_WINDOW_WIDTH,
                                 Window::DEFAULT_WINDOW_HEIGHT - 96 - 60));
    window->addChild(scroll);

    auto table = TextMenuTable::create();
    table->setModel(itemModels);
    table->setPosition(0, 0);
    table->setScroll(scroll);
    table->prepare();
    scroll->getScrollLayer()->addChild(table);
    scroll->setScrollSize(Size(scroll->getContentSize().width, table->getContentSize().height));
    scroll->scrollToTop();
    table->setOnTouchCallback([this](cocos2d::Node *node, SortItemModel_ptr model) {
        StateManager::getInstance()->getCurrent()->setEvent(model->getEvent());
    });
    
    this->window = window;
    AnimIn();
	return true;
}

void EffectsTestState::displayEffect(int effectId)
{
    auto effect = CommonEffect::create(effectId);
    effect->setResetDuration(currentDuration);
    effect->setPosition(this->getContentSize().width * 0.5, (this->getContentSize().height * 0.5) - 170);
    this->addChild(effect);
    effect->runAction(Sequence::create(effect->getPlayAction(),
                                       RemoveSelf::create(),
                                       nullptr));
}

void EffectsTestState::update(float delta)
{
    auto ev = this->popEvent();
    switch (ev.Name) {
        case Event::CLOSE:
        {
            Director::getInstance()->popScene();
            break;
        }
        case Event::DURATION_DOWN:
        {
            if (this->currentDuration > 0.1) {
                this->currentDuration = this->currentDuration - 0.1;

                auto label = (Label *)this->getChildByTag(2);
                if (label != nullptr) {
                    label->setString(StringUtils::format("Duration:%.2f", this->currentDuration));
                }
            }
            break;
        }
        case Event::DURATION_UP:
        {

            this->currentDuration = this->currentDuration + 0.1;

            auto label = (Label *)this->getChildByTag(2);
            if (label != nullptr) {
                label->setString(StringUtils::format("Duration:%.2f", this->currentDuration));
            }

            break;
        }
        case Event::PLAY:
        {
            displayEffect(activeEffectId);
            break;
        }
        default:
            if (ev.Name > effectRef) {
                activeEffectId = ev.Name - effectRef;
                currentDuration = MasterEffectModel::create(activeEffectId)->getDuration();
                auto durLabel = (Label *)this->getChildByTag(2);
                if (durLabel != nullptr) {
                    durLabel->setString(StringUtils::format("Duration:%.2f", this->currentDuration));
                }
                auto label = (Label *)this->getChildByTag(4);
                if (label != nullptr) {
                    label->setString(StringUtils::format("Id %d", this->activeEffectId));
                }
            }
            break;
    }
}
