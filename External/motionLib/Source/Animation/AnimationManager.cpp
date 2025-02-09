#include "AnimationManager.h"

using namespace MCBM;

Animation* MCBM::AnimationManager::GetAnimation(std::string name)
{
	auto itr = animations_.find(name);
	if (itr == animations_.end())
	{
		if (!animations_.empty()) return animations_.begin()->second.get();
		return nullptr;
	}
	return animations_[name].get();

};
void MCBM::AnimationManager::SetAnimation(std::unique_ptr<Animation> animation)
{
	animations_[animation->name] = std::move(animation);
}

MCBM::AnimationManager::AnimationManager()
{
}
