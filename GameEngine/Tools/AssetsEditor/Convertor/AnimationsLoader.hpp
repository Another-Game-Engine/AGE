#pragma once

// todo to pass in .cpp
#include <glm/gtc/quaternion.hpp>

#include "AssimpLoader.hpp"

#include <map>
#include <Skinning/Skeleton.hpp>
#include <AssetManagement/Data/AnimationData.hpp>
#include "ConvertorStatusManager.hpp"
#include "CookingTask.hpp"

namespace AGE
{
	class AnimationsLoader
	{
	public:
		static bool save(std::shared_ptr<CookingTask> cookinTask)
		{
			if (!cookinTask->dataSet->loadAnimations)
				return true;
			if (cookinTask->animations.empty())
				return true;
			auto tid = Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PushTask("Animation loader : saving " + cookinTask->dataSet->filePath.getShortFileName());

			auto folderPath = std::tr2::sys::path(cookinTask->serializedDirectory.path().directory_string() + "\\" + cookinTask->dataSet->filePath.getFolder());

			if (!std::tr2::sys::exists(folderPath) && !std::tr2::sys::create_directories(folderPath))
			{
				Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);
				std::cerr << "Animation convertor error : creating directory" << std::endl;
				return false;
			}
			auto fileName = cookinTask->dataSet->filePath.getShortFileName() + ".aage";
			auto name = cookinTask->serializedDirectory.path().directory_string() + "\\" + cookinTask->dataSet->filePath.getFolder() + fileName;

			std::ofstream ofs(name, std::ios::trunc | std::ios::binary);
			cereal::PortableBinaryOutputArchive ar(ofs);
			ar(*cookinTask->animations[0]);
			Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);
			return true;
		}

		static bool load(std::shared_ptr<CookingTask> cookinTask)
		{
			if (!cookinTask->dataSet->loadAnimations)
				return true;
			if (!cookinTask->assimpScene->HasAnimations())
				return false;
			auto tid = Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PushTask("Animation loader : loading " + cookinTask->dataSet->filePath.getShortFileName());

			cookinTask->animations.resize(cookinTask->assimpScene->mNumAnimations);
			for (unsigned int animNum = 0; animNum < cookinTask->assimpScene->mNumAnimations; ++animNum)
			{
				auto aiAnim = cookinTask->assimpScene->mAnimations[animNum];
				cookinTask->animations[animNum] = std::make_shared<AnimationData>();
				auto anim = cookinTask->animations[animNum];
				anim->name = aiAnim->mName.data;
				anim->duration = static_cast<float>(aiAnim->mDuration);
				anim->id = animNum;
				auto channelCounter = 0;
				for (unsigned int channelNbr = 0; channelNbr < aiAnim->mNumChannels; ++channelNbr)
				{
					auto aiChannel = aiAnim->mChannels[channelNbr];
					auto findBoneName = cookinTask->skeleton->bonesReferences.find(aiChannel->mNodeName.data);
					if (findBoneName == std::end(cookinTask->skeleton->bonesReferences))
						continue;
					anim->channels.resize(channelCounter + 1);
					auto &channel = anim->channels[channelCounter];
					unsigned int boneIndex = findBoneName->second;
					++channelCounter;
					channel.boneIndex = boneIndex;
					// we push positions
					for (unsigned int i = 0; i < aiChannel->mNumPositionKeys; ++i)
					{
						channel.translation.emplace_back(
							glm::vec3(aiChannel->mPositionKeys[i].mValue.x, aiChannel->mPositionKeys[i].mValue.y, aiChannel->mPositionKeys[i].mValue.z)
							, static_cast<float>(aiChannel->mPositionKeys[i].mTime));
						if (i > 0)
							channel.translation[i - 1].deltaTime = channel.translation[i].time - channel.translation[i - 1].time;
					}
					// we push scale
					for (unsigned int i = 0; i < aiChannel->mNumScalingKeys; ++i)
					{
						channel.scale.emplace_back(
							glm::vec3(aiChannel->mScalingKeys[i].mValue.x, aiChannel->mScalingKeys[i].mValue.y, aiChannel->mScalingKeys[i].mValue.z)
							, static_cast<float>(aiChannel->mScalingKeys[i].mTime));
						if (i > 0)
							channel.scale[i - 1].deltaTime = channel.scale[i].time - channel.scale[i - 1].time;
					}
					// we push rotation
					for (unsigned int i = 0; i < aiChannel->mNumRotationKeys; ++i)
					{
						channel.rotation.emplace_back(
							glm::normalize(glm::quat(aiChannel->mRotationKeys[i].mValue.w, aiChannel->mRotationKeys[i].mValue.x, aiChannel->mRotationKeys[i].mValue.y, aiChannel->mRotationKeys[i].mValue.z))
							, static_cast<float>(aiChannel->mRotationKeys[i].mTime));
						if (i > 0)
							channel.rotation[i - 1].deltaTime = channel.rotation[i].time - channel.rotation[i - 1].time;
					}
				}
			}
			Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);

			return true;
		}
	};
}