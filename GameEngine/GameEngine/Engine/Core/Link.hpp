#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Entities/EntityTypedef.hpp>
#include <cstring>
#include <array>

class AScene;

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

		void registerCullableId(std::size_t id);
		void unregisterCullableId(std::size_t id);

	private:
		void pushCommand();
	public:
		const glm::mat4 &getTransform();
	private:
		glm::vec3 _position;
		glm::vec3 _scale;
		glm::quat _orientation;
		glm::mat4 _trans;
		bool _computeTrans;
		std::array<std::size_t, MAX_CPT_NUMBER> _cullableLinks;
	public:
		void *_octree;
	public:
		Link();
		void reset();

		friend class AScene;
	};
}