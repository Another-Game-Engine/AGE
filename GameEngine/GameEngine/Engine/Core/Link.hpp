#pragma once

#include <Entities/EntityTypedef.hpp>
#include <Core/PrepareKey.hpp>
#include <vector>
#include <array>
#include <Utils/Serialization/QuaternionSerialization.hpp>
#include <Utils/Serialization/VectorSerialization.hpp>
#include <glm/gtc/quaternion.hpp>

namespace AGE
{
	class RenderScene;
	class AScene;
	class EntityData;
	struct Link
	{
		inline const glm::vec3 &getPosition() const { return _position; }
		inline const glm::vec3 &getScale() const { return _scale; }
		inline const glm::quat &getOrientation() const { return _orientation; }
		inline float *getPositionPtr() { return &_position.x; }
		inline float *getScalePtr() { return &_scale.x; }
		inline float *getOrientationPtr() { return &_orientation.x; }


		// Used by modules like physic, do not use it to set object position, use setPosition instead
		void internalSetPosition(const glm::vec3 &v);
		// Used by modules like physic, do not use it to set object scale, use setScale instead
		void internalSetScale(const glm::vec3 &v);
		// Used by modules like physic, do not use it to set object orientation, use setOrientation instead
		void internalSetOrientation(const glm::quat &v);
		// Used by modules like physic, do not use it to set object forward, use setForward instead
		void internalSetForward(const glm::vec3 &v);
		// Used by modules like physic, do not use it to set object transform, use setTransform instead
		void internalSetTransform(const glm::mat4 &t);
	
		inline bool isUserModified() const
		{
			return _userModification;
		}

		inline void setUserModified(bool tof)
		{
			_userModification = tof;
		}

		void setPosition(const glm::vec3 &v);
		void setForward(const glm::vec3 &v);
		void setScale(const glm::vec3 &v);
		void setScale(float v);
		void setOrientation(const glm::quat &v);
		void setTransform(const glm::mat4 &t);

		void registerOctreeObject(const PrepareKey &key);
		void unregisterOctreeObject(const PrepareKey &key);

		inline bool hasChildren() const { return _children.size() > 0; };
		inline bool hasParent() const { return _parent != nullptr; }
		inline bool hasParent(const Link *parent) const { return _parent == parent; }
		inline const std::vector<Link*> &getChildren() { return _children; }

		void attachChild(Link *child);
		void detachChild(Link *child);
		void detachChildren();
		bool hasChild(const Link *child) const;
		void attachParent(Link *parent);
		void detachParent();

		const glm::mat4 getGlobalTransform() const;
		const glm::mat4 &getGlobalTransform();
		const glm::mat4 getLocalTransform() const;
		const glm::mat4 &getLocalTransform();
	private:
		EntityData *_entityPtr;
		bool _userModification = false;

		glm::vec3 _position;
		glm::vec3 _scale;
		glm::quat _orientation;
		glm::mat4 _localTransformation;
		glm::mat4 _globalTransformation;

		bool _localDirty;
		std::vector<PrepareKey> _octreeObjects;
		Link *_parent;
		std::vector<Link*> _children;

		void _setChild(Link *ptr);
		void _setParent(Link *ptr);
		void _removeChild(Link *ptr);
		void _removeParent();
		void _detachFromRoot();
		void _attachToRoot();
		void _updateGlobalTransform();
	public:
		RenderScene *_renderScene;
	public:
		Link();
		Link(EntityData *entity, AScene *_scene);

		inline EntityData *getEntity() { return _entityPtr; }

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