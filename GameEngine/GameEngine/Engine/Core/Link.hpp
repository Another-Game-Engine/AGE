#pragma once

#include <glm/fwd.hpp>
#include <glm/gtc/quaternion.hpp>
#include <Entities/EntityTypedef.hpp>
#include <Core/PrepareKey.hpp>
#include <cstring>
#include <array>
#include <Utils/GlmSerialization.hpp>

class AScene;

namespace AGE
{
	class RenderScene;
	struct Link
	{
		inline const glm::vec3 &getPosition() const { return _position; }
		inline const glm::vec3 &getScale() const { return _scale; }
		inline const glm::quat &getOrientation() const { return _orientation; }
		inline float *getPositionPtr() { return glm::value_ptr(_position); }
		inline float *getScalePtr() { return glm::value_ptr(_scale); }
		inline float *getOrientationPtr() { return glm::value_ptr(_orientation); }


		// Used by modules like physic, do not use it to set object position, use setPosition instead
		void internalSetPosition(const glm::vec3 &v);
		// Used by modules like physic, do not use it to set object scale, use setScale instead
		void internalSetScale(const glm::vec3 &v);
		// Used by modules like physic, do not use it to set object orientation, use setOrientation instead
		void internalSetOrientation(const glm::quat &v);
		// Used by modules like physic, do not use it to set object forward, use setForward instead
		void internalSetForward(const glm::vec3 &v);

		inline bool userModified()
		{
			auto ret = _userModification; _userModification = false; return ret;
		}

		void setPosition(const glm::vec3 &v);
		void setForward(const glm::vec3 &v);
		void setScale(const glm::vec3 &v);
		void setOrientation(const glm::quat &v);

		void registerOctreeObject(const PrepareKey &key);
		void unregisterOctreeObject(const PrepareKey &key);

		inline bool hasChildren() const { return _lastChildrenIndex != 0; };
	public:
		const glm::mat4 &getTransform();
	private:
		bool _userModification = false;
		glm::vec3 _position;
		glm::vec3 _scale;
		glm::quat _orientation;
		glm::mat4 _trans;
		bool _computeTrans;
		std::array<PrepareKey, MAX_CPT_NUMBER> _octreeObjects;
		Link *_parent;
		std::array<Link*, MAX_CHILDREN> _children;
		std::size_t _lastOctreeObjectIndex;
		std::size_t _lastChildrenIndex;

		void _setChild(Link *ptr);
		void _setParent(Link *ptr);
		void _removeChild(Link *ptr);
		void _removeParent();
	public:
		RenderScene *_octree;
	public:
		Link();
		void reset();

		template < typename Archive >
		void serialize(Archive &ar)
		{
			ar(
				cereal::make_nvp("Position", _position)
				, cereal::make_nvp("Scale", _scale)
				, cereal::make_nvp("Orientation", _orientation)
				);
		}

		friend class AScene;
	};
}