#pragma once

#include <Components/Collider.hpp>

namespace AGE
{
	template <typename Archive>
	inline void Collider::save(Archive &ar, const std::uint32_t version) const
	{
		const Physics::ColliderType colliderType = getColliderType();
		ar(cereal::make_nvp("ColliderType", static_cast<std::uint8_t>(colliderType)));
		ar(cereal::make_nvp("Material", getMaterial()->getName()));
		ar(cereal::make_nvp("IsATrigger", isATrigger()));
		ar(cereal::make_nvp("FilterGroup", static_cast<std::uint8_t>(getFilterGroup())));
		switch (colliderType)
		{
			case Physics::ColliderType::Box:
				saveBoxCollider(ar, version);
				break;
			case Physics::ColliderType::Capsule:
				saveCapsuleCollider(ar, version);
				break;
			case Physics::ColliderType::Mesh:
				saveMeshCollider(ar, version);
				break;
			case Physics::ColliderType::Sphere:
				saveSphereCollider(ar, version);
				break;
			default:
				assert(!"Never reached");
				break;
		}
	}

	template <typename Archive>
	inline void Collider::saveBoxCollider(Archive &ar, const std::uint32_t version) const
	{
		ar(cereal::make_nvp("Center", getCenter()));
		ar(cereal::make_nvp("Size", getSize()));
	}

	template <typename Archive>
	inline void Collider::saveCapsuleCollider(Archive &ar, const std::uint32_t version) const
	{
		ar(cereal::make_nvp("Center", getCenter()));
		ar(cereal::make_nvp("Height", getHeight()));
		ar(cereal::make_nvp("Radius", getRadius()));
	}

	template <typename Archive>
	inline void Collider::saveMeshCollider(Archive &ar, const std::uint32_t version) const
	{
		std::shared_ptr<const MeshInstance> mesh = getMesh();
		ar(cereal::make_nvp("Mesh", mesh != nullptr ? mesh->path : ""));
		ar(cereal::make_nvp("Convex", isConvex()));
	}

	template <typename Archive>
	inline void Collider::saveSphereCollider(Archive &ar, const std::uint32_t version) const
	{
		ar(cereal::make_nvp("Center", getCenter()));
		ar(cereal::make_nvp("Radius", getRadius()));
	}

	template <typename Archive>
	inline void Collider::load(Archive &ar, const std::uint32_t version)
	{
		std::uint8_t colliderType;
		std::string material;
		bool isATrigger;
		std::uint8_t filterGroup;
		ar(cereal::make_nvp("ColliderType", colliderType));
		reset();
		init(static_cast<Physics::ColliderType>(colliderType));
		ar(cereal::make_nvp("Material", material));
		ar(cereal::make_nvp("IsATrigger", isATrigger));
		ar(cereal::make_nvp("FilterGroup", filterGroup));
		setMaterial(material);
		setAsTrigger(isATrigger);
		setFilterGroup(static_cast<Physics::FilterGroup>(filterGroup));
		switch (static_cast<Physics::ColliderType>(colliderType))
		{
			case Physics::ColliderType::Box:
				loadBoxCollider(ar, version);
				break;
			case Physics::ColliderType::Capsule:
				loadCapsuleCollider(ar, version);
				break;
			case Physics::ColliderType::Mesh:
				loadMeshCollider(ar, version);
				break;
			case Physics::ColliderType::Sphere:
				loadSphereCollider(ar, version);
				break;
			default:
				assert(!"Never reached");
				break;
		}
	}

	template <typename Archive>
	inline void Collider::loadBoxCollider(Archive &ar, const std::uint32_t version)
	{
		glm::vec3 center;
		glm::vec3 size;
		ar(cereal::make_nvp("Center", center));
		ar(cereal::make_nvp("Size", size));
		setCenter(center);
		setSize(size);
	}

	template <typename Archive>
	inline void Collider::loadCapsuleCollider(Archive &ar, const std::uint32_t version)
	{
		glm::vec3 center;
		float height;
		float radius;
		ar(cereal::make_nvp("Center", center));
		ar(cereal::make_nvp("Height", height));
		ar(cereal::make_nvp("Radius", radius));
		setCenter(center);
		setHeight(height);
		setRadius(radius);
	}

	template <typename Archive>
	inline void Collider::loadMeshCollider(Archive &ar, const std::uint32_t version)
	{
		std::string mesh;
		bool convex;
		ar(cereal::make_nvp("Mesh", mesh));
		setMesh(mesh);
		ar(cereal::make_nvp("Convex", convex));
		setAsConvex(convex);
	}

	template <typename Archive>
	inline void Collider::loadSphereCollider(Archive &ar, const std::uint32_t version)
	{
		glm::vec3 center;
		float radius;
		ar(cereal::make_nvp("Center", center));
		ar(cereal::make_nvp("Radius", radius));
		setCenter(center);
		setRadius(radius);
	}
}