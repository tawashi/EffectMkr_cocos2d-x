#pragma once

#include "State.h"

USING_NS_CC;

namespace engTrain
{
    class EffectsTestState : public State
    {
    public:
        enum Event : int {
            DURATION_DOWN = 1,
            DURATION_UP,
            PLAY,
            CLOSE = 99,
        };
        
        CREATE_FUNC(EffectsTestState);
        virtual bool init();
        virtual void update(float delta);
    private:
        static const int effectRef = 100;
        void displayEffect(int effectId);
        float currentDuration;
        int activeEffectId;
    };
}
