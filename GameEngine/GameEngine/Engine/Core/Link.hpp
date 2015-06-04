#pragma once

#include <Entities/EntityTypedef.hpp>
#include <Core/PrepareKey.hpp>
#include <vector>
#include <unordered_set>
#include <array>
#include <Utils/Serialization/QuaternionSerialization.hpp>
#include <Utils/Serialization/VectorSerialization.hpp>
#include <glm/gtc/quaternion.hpp>
#include <cereal/cereal.hpp>
#include <Configuration.hpp>

#include <BFC/BFCLink.hpp>


namespace AGE
{
	class RenderScene;
	class AScene;
	class EntityData;
	class BFCLinkTracker;
	class BFCBlockManager;

#ifdef AGE_BFC
	struct Link : public BFCLink
#else
	struct Link
#endif
	{
		inline const glm::vec3 &getPosition() const { return _position; }
		inline const glm::vec3 &getScale() const { return _scale; }
		inline const glm::quat &getOrientation() const { return _orientation; }
		inline float *getPositionPtr() { return &_position.x; }
		inline float *getScalePtr() { return &_scale.x; }
		inline float *getOrientationPtr() { return &_orientation.x; }


		// Used by modules like physic, do not use it to set object position, use setPosition instead
		void internalSetPosition(const glm::vec3 &v, bool recalculate);
		// Used by modules like physic, do not use it to set object scale, use setScale instead
		void internalSetScale(const glm::vec3 &v, bool recalculate);
		// Used by modules like physic, do not use it to set object orientation, use setOrientation instead
		void internalSetOrientation(const glm::quat &v, bool recalculate);
		// Used by modules like physic, do not use it to set object forward, use setForward instead
		void internalSetForward(const glm::vec3 &v, bool recalculate);
		// Used by modules like physic, do not use it to set object transform, use setTransform instead
		void internalSetTransform(const glm::mat4 &t, bool recalculate);

		inline bool isUserModified() const
		{
			return _userModification;
		}

		inline void setUserModified(bool tof)
		{
			_userModification = tof;
		}

		void setPosition(const glm::vec3 &v, bool recalculate = true);
		void setForward(const glm::vec3 &v, bool recalculate = true);
		void setScale(const glm::vec3 &v, bool recalculate = true);
		void setScale(float v, bool recalculate = true);
		void setOrientation(const glm::quat &v, bool recalculate = true);
		void setTransform(const glm::mat4 &t, bool recalculate = true);

		void registerOctreeObject(const PrepareKey &key);
		void unregisterOctreeObject(const PrepareKey &key);

		bool hasChildren() const;
		bool hasParent() const;
		bool hasParent(const Link *parent) const;
		bool isAttachedToRoot() const;
		const std::vector<Link*> &getChildren() const;

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
#ifdef AGE_BFC
		void BFC_ADD();
		void BFC_REMOVE();
#else
		void BFC_ADD(){}
		void BFC_REMOVE(){}
#endif

		EntityData *_entityPtr;
		bool _userModification = false;

		glm::vec3 _position;
		glm::vec3 _scale;
		glm::quat _orientation;
		glm::mat4 _localTransformation;
		glm::mat4 _globalTransformation;

		bool _localDirty;
		std::unordered_set<PrepareKey> _octreeObjects;
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
		void serialize(Archive &ar, const std::uint32_t version)
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

CEREAL_CLASS_VERSION(AGE::Link, 0)