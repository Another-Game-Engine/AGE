#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Entities/EntityTypedef.hpp>

#include <cstring>

namespace AGE
{
	struct Link
	{
		const glm::vec3 &getPosition() const { return _position; }
		const glm::vec3 &getScale() const { return _scale; }
		const glm::quat &getOrientation() const { return _orientation; }

		void setPosition(const glm::vec3 &v) { _computeTrans = true; _position = v; }
		void setScale(const glm::vec3 &v) { _computeTrans = true; _scale = v; }
		void setOrientation(const glm::quat &v) { _computeTrans = true; _orientation = v; }

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
		std::uint8_t _cullableSize;
		ENTITY_ID _cullableLinks[MAX_CPT_NUMBER];
	public:
		Link()
			: _position(0)
			, _scale(1)
			, _orientation(glm::mat4(1))
			, _trans(1)
			, _computeTrans(true)
			, _cullableSize(0)
		{
			std::memset(_cullableLinks, 0, sizeof(ENTITY_ID));
		}
	};
}