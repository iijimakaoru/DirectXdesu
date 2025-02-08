#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <Math/MVector3.h>
#include <Math/MQuaternion.h>
#include <memory>

namespace MCBM
{
    typedef struct NodeAnim
    {
        std::string name;
        std::vector<MVector3> position;
        std::vector<double> positionTime;
        std::vector<MQuaternion> rotation;
        std::vector<double> rotationTime;
        std::vector<MVector3> scale;
        std::vector<double> scaleTime;
    }NodeAnim;

    typedef struct Animation
    {
        std::string name;
        double duration;
        double ticksPerSecond;
        std::vector<NodeAnim> channels;
        std::string animationRootNode = "NULL";
    }Animation;


    class AnimationManager
    {
        std::unordered_map<std::string, std::unique_ptr<Animation>> animations_;
    public:
        Animation* GetAnimation(std::string name); //アニメーションを取得

        void SetAnimation(std::unique_ptr<Animation> animation);//アニメーションを追加
        AnimationManager();
        AnimationManager(const AnimationManager&) = delete;
        AnimationManager& operator= (const AnimationManager&) = delete;
    };
}