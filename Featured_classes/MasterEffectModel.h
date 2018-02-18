#pragma once
#include "MasterModelBase.h"

namespace engTrain
{
    class MasterEffectModel : public MasterModelBase
    {
    public:
        static std::shared_ptr<MasterEffectModel> create(int id);
        
        CC_SYNTHESIZE(int, _id, Id);
        CC_SYNTHESIZE(std::string, _name, Name);
        CC_SYNTHESIZE(std::string, _filename, Filename);
        CC_SYNTHESIZE(int, _width, Width);
        CC_SYNTHESIZE(int, _height, Height);
        CC_SYNTHESIZE(float, _duration, Duration);
        CC_SYNTHESIZE(int, _scale, Scale);
        CC_SYNTHESIZE(int, _diffY, DiffY);
        CC_SYNTHESIZE(cocos2d::Color3B, _color, Color);
        CC_SYNTHESIZE(std::vector<int>, _frames, Frames);
        CC_SYNTHESIZE(std::string, _sound, Sound);
        
    private:
        static std::vector<int> parseFrame(const std::string& frames);
    };
    
    typedef std::shared_ptr<MasterEffectModel> MasterEffectModel_ptr;
}
