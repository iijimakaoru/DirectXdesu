#pragma once
#include<Animation/Skelton.h>
#include<unordered_map>
#include<memory>

namespace MCBM
{
	class AnimationModelManager
	{
		Skelton* _Load(const std::string& modelname, const char* pName);

		Skelton* _Find(const std::string& modelname, const char* pName);
		
		std::unordered_map<size_t, size_t>						modelNames_;
		
		std::unordered_map<size_t, std::unique_ptr<Skelton>>		models_;


	public:
		static AnimationModelManager* GetInstance();

	private:
		AnimationModelManager() = default;
		~AnimationModelManager() = default;
		AnimationModelManager(const AnimationModelManager&) = delete;
		const AnimationModelManager& operator=(const AnimationModelManager&) = delete;


	public:
		MCBM::Skelton* GetModel(const std::string& name);
		Skelton* Load(const std::string& modelname, const char* pName = nullptr);
	};
}
