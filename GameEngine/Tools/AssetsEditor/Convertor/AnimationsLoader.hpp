#pragma once

// todo to pass in .cpp
#include <glm/gtc/quaternion.hpp>

#include "AssimpLoader.hpp"

#include <map>
#include <Skinning/Skeleton.hpp>
#include <Skinning/Animation.hpp>
#include "ConvertorStatusManager.hpp"

namespace AGE
{
	class AnimationsLoader
	{
	public:
		static bool save(std::shared_ptr<AssetDataSet> dataSet)
		{
			if (dataSet->animations.empty())
				return true;
			auto tid = Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PushTask("Animation loader : saving " + dataSet->filePath.getShortFileName());

			auto folderPath = std::tr2::sys::path(dataSet->serializedDirectory.path().directory_string() + "\\" + dataSet->filePath.getFolder());

			if (!std::tr2::sys::exists(folderPath) && !std::tr2::sys::create_directories(folderPath))
			{
				Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);
				std::cerr << "Animation convertor error : creating directory" << std::endl;
				return false;
			}
			auto fileName = dataSet->animationName.empty() ? dataSet->filePath.getShortFileName() + ".aage" : dataSet->animationName + ".aage";
			auto name = dataSet->serializedDirectory.path().directory_string() + "\\" + dataSet->filePath.getFolder() + fileName;

			std::ofstream ofs(name, std::ios::trunc | std::ios::binary);
			cereal::PortableBinaryOutputArchive ar(ofs);
			ar(*dataSet->animations[0]);
			Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);
			return true;
		}

		static bool load(std::shared_ptr<AssetDataSet> dataSet)
		{
			if (!dataSet->assimpScene->HasAnimations())
				return false;
			auto tid = Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PushTask("Animation loader : loading " + dataSet->filePath.getShortFileName());

			dataSet->animations.resize(dataSet->assimpScene->mNumAnimations);
			for (unsigned int animNum = 0; animNum < dataSet->assimpScene->mNumAnimations; ++animNum)
			{
				auto aiAnim = dataSet->assimpScene->mAnimations[animNum];
				dataSet->animations[animNum] = std::make_shared<Animation>();
				auto anim = dataSet->animations[animNum];
				anim->name = aiAnim->mName.data;
				anim->duration = aiAnim->mDuration;
				anim->id = animNum;
				auto channelCounter = 0;
				for (unsigned int channelNbr = 0; channelNbr < aiAnim->mNumChannels; ++channelNbr)
				{
					auto aiChannel = aiAnim->mChannels[channelNbr];
					auto findBoneName = dataSet->skeleton->bonesReferences.find(aiChannel->mNodeName.data);
					if (findBoneName == std::end(dataSet->skeleton->bonesReferences))
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
							, aiChannel->mPositionKeys[i].mTime);
						if (i > 0)
							channel.translation[i - 1].deltaTime = channel.translation[i].time - channel.translation[i - 1].time;
					}
					// we push scale
					for (unsigned int i = 0; i < aiChannel->mNumScalingKeys; ++i)
					{
						channel.scale.emplace_back(
							glm::vec3(aiChannel->mScalingKeys[i].mValue.x, aiChannel->mScalingKeys[i].mValue.y, aiChannel->mScalingKeys[i].mValue.z)
							, aiChannel->mScalingKeys[i].mTime);
						if (i > 0)
							channel.scale[i - 1].deltaTime = channel.scale[i].time - channel.scale[i - 1].time;
					}
					// we push rotation
					for (unsigned int i = 0; i < aiChannel->mNumRotationKeys; ++i)
					{
						channel.rotation.emplace_back(
							glm::normalize(glm::quat(aiChannel->mRotationKeys[i].mValue.w, aiChannel->mRotationKeys[i].mValue.x, aiChannel->mRotationKeys[i].mValue.y, aiChannel->mRotationKeys[i].mValue.z))
							, aiChannel->mRotationKeys[i].mTime);
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