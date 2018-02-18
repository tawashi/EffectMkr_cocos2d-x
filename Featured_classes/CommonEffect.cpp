#include "CommonEffect.h"
#include "SE.h"

USING_NS_CC;
USING_NS_ENGTRAIN;

const float CommonEffect::WIDTH  = 170;
const float CommonEffect::HEIGHT = 170;

CommonEffect* CommonEffect::create(int effectId)
{
    CommonEffect *pRet = new CommonEffect();
    if (pRet && pRet->init(effectId)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool CommonEffect::init(int effectId)
{
    if (!Node::init()) {
        return false;
    }

    model = MasterEffectModel::create(effectId);
    this->setColor(model->getColor());

    auto effect = Sprite::create(StringUtils::format("%s%s",model->getFilename().c_str(),".png"), Rect(WIDTH * 0, HEIGHT * 0, 1, 1));
    effect->setTag(1);
    BlendFunc blend;
    blend.src = GL_SRC_ALPHA;
    blend.dst = GL_ONE;
    effect->setBlendFunc(blend);
    effect->setScale(model->getScale());
    effect->setVisible(false);
    //effect->setAnchorPoint(Point(0.5,0));
    this->addChild(effect);
    
    return true;
}

void CommonEffect::play(cocos2d::CallFunc* complete)
{
    this->runAction(Sequence::create(this->getPlayAction(),
                                     complete,
                                     RemoveSelf::create(),
                                     nullptr));
}

FiniteTimeAction* CommonEffect::getPlayAction()
{
    ET_LOG("[CommonEffect] play");
    
    auto effect = this->getChildByTag(1);
    auto anim = Animation::create();

    //Setting frames(if getFrames is empty add all sprites)
    if (model->getFrames().size() > 0) {
        for(auto frame : model->getFrames()) {
            int x = frame % model->getWidth();
            int y = frame / model->getWidth();

            Rect a = Rect(WIDTH * x, HEIGHT  * y, WIDTH, HEIGHT);
            auto sf = CCSpriteFrame::create(StringUtils::format("%s%s", model->getFilename().c_str(), ".png"), Rect(WIDTH * x, HEIGHT * y, WIDTH, HEIGHT));
            anim->addSpriteFrame(sf);
        }
    } else {
        for(int y=0;y<model->getHeight();y++){
            for (int x=0; x < model->getWidth(); x++) {
                anim->addSpriteFrame(CCSpriteFrame::create(StringUtils::format("%s%s", model->getFilename().c_str(), ".png"),
                                                           Rect(WIDTH * x, HEIGHT * y, WIDTH, HEIGHT)));
            }
        }
    }

    //Set Y offset(Flashのデータから作成されてるので y が逆になります)
    this->setPositionY(this->getPositionY() - model->getDiffY());

    //Resetting duration if necessary
    float animDuration = _resetDuration != 0 ? _resetDuration : model->getDuration();
    anim->setDelayPerUnit(animDuration/ (model->getWidth() * model->getHeight()));

    ET_LOG("CommonEffect duration:%f",anim->getDuration());
    
    if (model->getSound() != "") {
        SE::playStream(model->getSound().c_str(), 0.8);
    }
    
    return Sequence::create(TargetedAction::create(effect, Show::create()),
                            TargetedAction::create(effect, Animate::create(anim)),
                            nullptr);
}
