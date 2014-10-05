#pragma once

#include <glm/fwd.hpp>
#include <glm/gtc/quaternion.hpp>
#include <Entities/EntityTypedef.hpp>
#include <Core/PrepareKey.hpp>
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

		void setPosition(const glm::vec3 &v);
		void setForward(const glm::vec3 &v);
		void setScale(const glm::vec3 &v);
		void setOrientation(const glm::quat &v);

		void registerOctreeObject(const PrepareKey &key);
		void unregisterOctreeObject(const PrepareKey &key);
	public:
		const glm::mat4 &getTransform();
	private:
		glm::vec3 _position;
		glm::vec3 _scale;
		glm::quat _orientation;
		glm::mat4 _trans;
		bool _computeTrans;
		std::array<PrepareKey, MAX_CPT_NUMBER> _octreeObjects;
	public:
		void *_octree;
	public:
		Link();
		void reset();

		friend class AScene;
	};
}