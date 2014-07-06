#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Entities/EntityTypedef.hpp>
#include <Core/Octree.hpp>
#include <cstring>

namespace AGE
{
	struct Link
	{
		const glm::vec3 &getPosition() const { return _position; }
		const glm::vec3 &getScale() const { return _scale; }
		const glm::quat &getOrientation() const { return _orientation; }

		void setPosition(const glm::vec3 &v) { _computeTrans = true; _position = v; pushCommand(); }
		void setScale(const glm::vec3 &v) { _computeTrans = true; _scale = v; pushCommand(); }
		void setOrientation(const glm::quat &v) { _computeTrans = true; _orientation = v; pushCommand(); }

		void registerCullableId(std::size_t id)
		{
			for (auto &b : _cullableLinks)
			{
				if (b == std::size_t(-1))
				{
					b = id;
					return;
				}
			}
			assert(false);
		}

		void unregisterCullableId(std::size_t id)
		{
			for (auto &b : _cullableLinks)
			{
				if (b == id)
				{
					b = std::size_t(-1);
					return;
				}
			}
			assert(false);
		}

	private:
		void pushCommand()
		{
			_octree->pushCommand(_position, _scale, _orientation, _cullableLinks);
		}
	public:

		const glm::mat4 &getTransform()
		{
			if (_computeTrans)
			{
				_trans = glm::mat4(1);
				_trans = glm::translate(_trans, _position);
				_trans = _trans * glm::toMat4(_orientation);
				_trans = glm::scale(_trans, _scale);
				_computeTrans = false;
			}
			return _trans;
		}
	private:
		glm::vec3 _position;
		glm::vec3 _scale;
		glm::quat _orientation;
		glm::mat4 _trans;
		bool _computeTrans;
		std::array<std::size_t, MAX_CPT_NUMBER> _cullableLinks;
	public:
		Octree *_octree;
	public:
		Link()
		{
			reset();
		}

		void reset()
		{
			_position = glm::vec3(0);
			_scale = glm::vec3(1);
			_orientation = glm::quat(glm::mat4(1));
			_trans = glm::mat4(1);
			_computeTrans = true;
			_cullableLinks.fill(std::size_t(-1));
		}

		friend class AScene;
	};
}