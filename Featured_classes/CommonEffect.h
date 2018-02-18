#pragma once

#include "Effect.h"
#include "MasterEffectModel.h"

namespace engTrain
{
    class CommonEffect : public Effect
    {
    public:
        static CommonEffect* create(int effectId);
        virtual bool init(int effectId);
        virtual void play(cocos2d::CallFunc* complete);
        cocos2d::FiniteTimeAction* getPlayAction();
        CC_SYNTHESIZE(float, _resetDuration, ResetDuration);

    private:
        static const float WIDTH;
        static const float HEIGHT;
        
        MasterEffectModel_ptr model;
        float resetDuration;
    };
}
