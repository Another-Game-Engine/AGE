#pragma once

#include "AssimpLoader.hpp"

#include <map>
#include <Skinning/Skeleton.hpp>

namespace AGE
{
	class SkeletonLoader
	{
	public:
		static bool load(AssetDataSet &dataSet)
		{
			if (!dataSet.assimpScene)
			{
				if (!AssimpLoader::Load(dataSet))
					return false;
			}
			auto boneOrigin = dataSet.assimpScene->mRootNode;
			if (dataSet.skeletonLoaded)
			{
				std::cerr << "Skeleton loader : skeleton [" << dataSet.skeleton->name << "] already exists." << std::endl;
				return false;
			}
			dataSet.skeleton = new Skeleton();
			Skeleton *skeleton = dataSet.skeleton;
			skeleton->name = dataSet.name;
			skeleton->inverseGlobal = glm::inverse(AssimpLoader::aiMat4ToGlm(boneOrigin->mTransformation));
			loadSkeletonFromAssimp(skeleton, boneOrigin, (unsigned int)(-1));

			for (unsigned int meshIndex = 0; meshIndex < dataSet.assimpScene->mNumMeshes; ++meshIndex)
			{
				aiMesh *mesh = dataSet.assimpScene->mMeshes[meshIndex];

				for (unsigned int i = 0; i < mesh->mNumBones; ++i)
				{
					std::string boneName = mesh->mBones[i]->mName.data;
					if (skeleton->bonesReferences.find(boneName) == std::end(skeleton->bonesReferences))
						continue;
					skeleton->bones[skeleton->bonesReferences[boneName]].offset = AssimpLoader::aiMat4ToGlm(mesh->mBones[i]->mOffsetMatrix);
				}
			}


			//we fill bone hierarchy
			for (unsigned int i = 0; i < skeleton->bones.size(); ++i)
			{
				aiNode *bonenode = dataSet.assimpScene->mRootNode->FindNode(aiString(skeleton->bones[i].name));
				if (!bonenode)
					continue;

				//we set bone transformation
				skeleton->bones[i].transformation = AssimpLoader::aiMat4ToGlm(bonenode->mTransformation);

				// we set parent
				if (bonenode->mParent != nullptr && skeleton->bonesReferences.find(bonenode->mParent->mName.data) == std::end(skeleton->bonesReferences))
				{
					auto parent = bonenode->mParent;
					while (parent && skeleton->bonesReferences.find(parent->mName.data) == std::end(skeleton->bonesReferences))
					{
						skeleton->bones[i].offset = glm::inverse(AssimpLoader::aiMat4ToGlm(parent->mTransformation)) * skeleton->bones[i].offset;
						skeleton->bones[i].transformation = AssimpLoader::aiMat4ToGlm(parent->mTransformation) * skeleton->bones[i].transformation;
						parent = parent->mParent;
					}
					if (parent)
					{
						skeleton->bones[i].parent = skeleton->bonesReferences.find(parent->mName.data)->second;
					}
					else
					{
						skeleton->firstBone = i;
					}
				}
				else if (bonenode->mParent)
				{
					skeleton->bones[i].parent = skeleton->bonesReferences.find(bonenode->mParent->mName.data)->second;
				}

				//we set children
				for (unsigned int c = 0; c < bonenode->mNumChildren; ++c)
				{
					auto f = skeleton->bonesReferences.find(bonenode->mChildren[c]->mName.data);
					if (f == std::end(skeleton->bonesReferences))
						continue;
					skeleton->bones[i].children.push_back(f->second);
				}
			}
			if (skeleton->bones.size() == 0)
			{
				std::cerr << "Skeleton loader : assets does not contain any skeleton." << std::endl;
				delete dataSet.skeleton;
				dataSet.skeletonLoaded = false;
				return false;
			}
			dataSet.skeletonLoaded = true;
			return true;
		}
	private:
		static void loadSkeletonFromAssimp(Skeleton *_skeleton, aiNode *_node, unsigned int _parent)
		{
			if (_skeleton->bonesReferences.find(_node->mName.data) != std::end(_skeleton->bonesReferences))
				return;
			auto index = _skeleton->bones.size();
			_skeleton->bones.push_back(AGE::Bone());
			_skeleton->bones.back().index = index;
			_skeleton->bones.back().name = _node->mName.data;
			_skeleton->bones.back().transformation = AssimpLoader::aiMat4ToGlm(_node->mTransformation);
			_skeleton->bones.back().offset = glm::inverse(AssimpLoader::aiMat4ToGlm(_node->mTransformation));
			_skeleton->bonesReferences.insert(std::make_pair(_node->mName.data, index));

			for (unsigned int a = 0; a < _node->mNumChildren; a++)
			{
				loadSkeletonFromAssimp(_skeleton, _node->mChildren[a], index);
			}
		}
	};
}