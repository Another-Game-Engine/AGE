#pragma once

#include <cstdint>
#include <string>

#include <glm/glm.hpp>

#include "fbx/FBXSceneEncoder.h"

namespace AGE
{
	class Node
	{
	public:

		static const std::uint32_t INVALID_ID = (std::uint32_t)(-1);
		static inline bool IS_VALID_ID(std::uint32_t i) { return INVALID_ID != i; }

		Node()
			: _id(INVALID_ID)
			, _name("")
			, _parent(INVALID_ID)
		{

		}

		void addChild(std::uint32_t id)
		{
			assert(id != INVALID_ID);
			_children.push_back(id);
		}

		void setParent(std::uint32_t id)
		{
			assert(id != INVALID_ID);
			_parent = id;
		}

		std::uint32_t _id;
		std::string _name;
		std::uint32_t _parent;
		std::vector<std::uint32_t> _children;
		glm::mat4 _worldMatrixReference;
		glm::mat4 _boneMatrixReference;
		glm::mat4 _worldTransform;
		glm::mat4 _boneTransform;
	};
}