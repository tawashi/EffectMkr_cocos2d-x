#include "MasterEffectModel.h"
#include "Converter.h"
#include "StringUtil.h"
#include "MasterDataManager.h"

USING_NS_CC;
USING_NS_ENGTRAIN;

//debug
std::shared_ptr<MasterEffectModel> MasterEffectModel::create(int id)
{
    auto map = MasterDataManager::getInstance()->load(MasterDataManager::TYPE_EFFECT, id);
    auto model = std::make_shared<MasterEffectModel>();
    model->setId(id);
    model->setName(map["name"]);
    model->setFilename(map["filename"]);
    model->setWidth(Converter::toInt(map["h"]));
    model->setHeight(Converter::toInt(map["v"]));
    model->setDuration(Converter::toFloat(map["duration"]));
    model->setScale(Converter::toInt(map["scale"]));
    model->setDiffY(Converter::toInt(map["y"]));
    model->setColor(Color3B(Converter::toInt(map["r"]), Converter::toInt(map["g"]), Converter::toInt(map["b"])));
    
    std::vector<int> frames = MasterEffectModel::parseFrame(map["frames"]);
    model->setFrames(frames);
    
    model->setSound(map["sound"]);
    
    return model;
}

std::vector<int> MasterEffectModel::parseFrame(const std::string& frames)
{
    std::vector<int> result;
    if (frames.empty()) {
        return result;
    }
    std::vector<std::string> parsed = StringUtil::split(frames, ',');
    
    for (auto frame : parsed) {
        if(frame.find('-') == std::string::npos) {
            result.push_back(Converter::toInt(frame));
            continue;
        }

        frame.erase(std::find(frame.begin(), frame.end(), '['));
        frame.erase(std::find(frame.begin(), frame.end(), ']'));

        std::vector<std::string> continuity = StringUtil::split(frame, '-');

        int first = Converter::toInt(continuity.at(0));
        int last  = Converter::toInt(continuity.at(1));
        int index = first;

        while(index != last) {
            result.push_back(index);
            if (first < last) {
                index++;
            } else {
                index--;
            }
        }
        result.push_back(last);
    }
    
    return result;
}
