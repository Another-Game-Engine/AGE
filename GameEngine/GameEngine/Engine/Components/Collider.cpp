#include <Components/Collider.hpp>
#include <Components/RigidBody.hpp>
#include <Components/PhysicsData.hpp>
#include <Core/AScene.hh>
#include <Physics/PhysicsInterface.hpp>
#include <Physics/WorldInterface.hpp>
#include <AssetManagement/AssetManager.hh>

#include <FileUtils/AssetFiles/CookedFileFilter.hpp>
#include <FileUtils/AssetFiles/Folder.hpp>
#include <FileUtils/AssetFiles/CookedFile.hpp>
#include <FileUtils/FileUtils/FileSystemHelpers.hpp>

#include <Utils/MatrixConversion.hpp>

#ifdef EDITOR_ENABLED
#include <imgui/imgui.h>
#endif

namespace AGE
{
	// Methods
	void Collider::init(Physics::ColliderType colliderType, const std::string &mesh)
	{
		SCOPE_profile_cpu_function("Physic");

		assert(collider == nullptr && "Collider already initialized");
		collider = entity->getScene()->getInstance<Physics::PhysicsInterface>()->getWorld()->createCollider(colliderType, mesh, entity->addComponent<Private::PhysicsData>(entity)->getData());
		collider->collider = this;
		scale(entity->getLink().getScale());
	}

	void Collider::postUnserialization()
	{
		//scale(entity->getLink().getScale());
	}

	void Collider::setMaterial(const std::string &material)
	{
		SCOPE_profile_cpu_function("Physic");

		assert(collider != nullptr && "Invalid Collider");
		collider->setMaterial(material);
	}

	Physics::MaterialInterface *Collider::getMaterial(void)
	{
		assert(collider != nullptr && "Invalid Collider");
		return collider->getMaterial();
	}

	const Physics::MaterialInterface *Collider::getMaterial(void) const
	{
		assert(collider != nullptr && "Invalid Collider");
		return collider->getMaterial();
	}

	Physics::ColliderType Collider::getColliderType(void) const
	{
		assert(collider != nullptr && "Invalid Collider");
		return collider->getColliderType();
	}

	void Collider::setAsTrigger(bool mustBeATrigger)
	{
		SCOPE_profile_cpu_function("Physic");

		assert(collider != nullptr && "Invalid Collider");
		collider->setAsTrigger(mustBeATrigger);
	}

	bool Collider::isATrigger(void) const
	{
		assert(collider != nullptr && "Invalid Collider");
		return collider->isATrigger();
	}

	void Collider::setFilterGroup(const std::string &filterName)
	{
		SCOPE_profile_cpu_function("Physic");

		assert(collider != nullptr && "Invalid Collider");
		collider->setFilterGroup(collider->getWorld()->getFilterGroupForFilterName(filterName));
	}

	void Collider::setFilterGroup(Physics::FilterGroup group)
	{
		SCOPE_profile_cpu_function("Physic");

		assert(collider != nullptr && "Invalid Collider");
		collider->setFilterGroup(group);
	}

	const std::string &Collider::getFilterGroupName(void) const
	{
		assert(collider != nullptr && "Invalid Collider");
		return collider->getWorld()->getFilterNameForFilterGroup(collider->getFilterGroup());
	}

	Physics::FilterGroup Collider::getFilterGroup(void) const
	{
		assert(collider != nullptr && "Invalid Collider");
		return collider->getFilterGroup();
	}

	void Collider::setCenter(const glm::vec3 &center)
	{
		assert(collider != nullptr && "Invalid Collider");
		switch (getColliderType())
		{
			case Physics::ColliderType::Box:
				collider->as<Physics::ColliderType::Box>()->setCenter(center);
				break;
			case Physics::ColliderType::Capsule:
				collider->as<Physics::ColliderType::Capsule>()->setCenter(center);
				break;
			case Physics::ColliderType::Sphere:
				collider->as<Physics::ColliderType::Sphere>()->setCenter(center);
				break;
			default:
				assert(!"Invalid collider type");
				break;
		}
	}

	glm::vec3 Collider::getCenter(void) const
	{
		assert(collider != nullptr && "Invalid Collider");
		switch (getColliderType())
		{
			case Physics::ColliderType::Box:
				return collider->as<Physics::ColliderType::Box>()->getCenter();
			case Physics::ColliderType::Capsule:
				return collider->as<Physics::ColliderType::Capsule>()->getCenter();
			case Physics::ColliderType::Sphere:
				return collider->as<Physics::ColliderType::Sphere>()->getCenter();
			default:
				assert(!"Invalid collider type");
				return glm::vec3();
		}

	}

	void Collider::setHeight(float height)
	{
		assert(collider != nullptr && "Invalid Collider");
		switch (getColliderType())
		{
			case Physics::ColliderType::Capsule:
				collider->as<Physics::ColliderType::Capsule>()->setHeight(height);
				break;
			default:
				assert(!"Invalid collider type");
				break;
		}
	}

	float Collider::getHeight(void) const
	{
		assert(collider != nullptr && "Invalid Collider");
		switch (getColliderType())
		{
			case Physics::ColliderType::Capsule:
				return collider->as<Physics::ColliderType::Capsule>()->getHeight();
			default:
				assert(!"Invalid collider type");
				return 0.0f;
		}
	}

	void Collider::setRadius(float radius)
	{
		assert(collider != nullptr && "Invalid Collider");
		switch (getColliderType())
		{
			case Physics::ColliderType::Capsule:
				collider->as<Physics::ColliderType::Capsule>()->setRadius(radius);
				break;
			case Physics::ColliderType::Sphere:
				collider->as<Physics::ColliderType::Sphere>()->setRadius(radius);
				break;
			default:
				assert(!"Invalid collider type");
				break;
		}
	}

	float Collider::getRadius(void) const
	{
		assert(collider != nullptr && "Invalid Collider");
		switch (getColliderType())
		{
			case Physics::ColliderType::Capsule:
				return collider->as<Physics::ColliderType::Capsule>()->getRadius();
			case Physics::ColliderType::Sphere:
				return collider->as<Physics::ColliderType::Sphere>()->getRadius();
			default:
				assert(!"Invalid collider type");
				return 0.0f;
		}
	}

	void Collider::setSize(const glm::vec3 &size)
	{
		assert(collider != nullptr && "Invalid Collider");
		switch (getColliderType())
		{
			case Physics::ColliderType::Box:
				collider->as<Physics::ColliderType::Box>()->setSize(size);
				break;
			default:
				assert(!"Invalid collider type");
				break;
		}
	}

	glm::vec3 Collider::getSize(void) const
	{
		assert(collider != nullptr && "Invalid Collider");
		switch (getColliderType())
		{
			case Physics::ColliderType::Box:
				return collider->as<Physics::ColliderType::Box>()->getSize();
			default:
				assert(!"Invalid collider type");
				return glm::vec3();
		}
	}

	void Collider::setMesh(const std::string &mesh)
	{
		assert(collider != nullptr && "Invalid Collider");
		switch (getColliderType())
		{
			case Physics::ColliderType::Mesh:
				collider->as<Physics::ColliderType::Mesh>()->setMesh(mesh);
				break;
			default:
				assert(!"Invalid collider type");
				break;
		}
	}

	const std::string &Collider::getMesh(void) const
	{
		assert(collider != nullptr && "Invalid Collider");
		switch (getColliderType())
		{
			case Physics::ColliderType::Mesh:
				return collider->as<Physics::ColliderType::Mesh>()->getMesh();
			default:
				assert(!"Invalid collider type");
				static std::string emptyString;
				return emptyString;
		}
	}

	void Collider::setAsConvex(void)
	{
		assert(collider != nullptr && "Invalid Collider");
		switch (getColliderType())
		{
			case Physics::ColliderType::Mesh:
				if (isConcave())
				{
					collider->as<Physics::ColliderType::Mesh>()->setAsConvex();
					scale(entity->getLink().getScale());
				}
				break;
			default:
				assert(!"Invalid collider type");
				break;
		}
	}

	void Collider::setAsConcave(void)
	{
		assert(collider != nullptr && "Invalid Collider");
		switch (getColliderType())
		{
			case Physics::ColliderType::Mesh:
				if (isConvex())
				{
					collider->as<Physics::ColliderType::Mesh>()->setAsConcave();
					scale(entity->getLink().getScale());
				}
				break;
			default:
				assert(!"Invalid collider type");
				break;
		}
	}

	bool Collider::isConvex(void) const
	{
		assert(collider != nullptr && "Invalid Collider");
		switch (getColliderType())
		{
			case Physics::ColliderType::Mesh:
				return collider->as<Physics::ColliderType::Mesh>()->isConvex();
			default:
				assert(!"Invalid collider type");
				return true;
		}
	}

	bool Collider::isConcave(void) const
	{
		assert(collider != nullptr && "Invalid Collider");
		switch (getColliderType())
		{
			case Physics::ColliderType::Mesh:
				return collider->as<Physics::ColliderType::Mesh>()->isConcave();
			default:
				assert(!"Invalid collider type");
				return false;
		}
	}

	void Collider::scale(const glm::vec3 &scaling)
	{
		assert(collider != nullptr && "Invalid Collider");
		collider->scale(scaling);
	}

	void Collider::setPosition(const glm::vec3 &position)
	{
		assert(collider != nullptr && "Invalid Collider");
		collider->setPosition(position);
	}

	void Collider::setRotation(const glm::quat &rotation)
	{
		assert(collider != nullptr && "Invalid Collider");
		collider->setRotation(rotation);
	}

	// Inherited Methods
	void Collider::reset(void)
	{
		SCOPE_profile_cpu_function("Physic");

		if (collider != nullptr)
		{
			entity->getScene()->getInstance<Physics::PhysicsInterface>()->getWorld()->destroyCollider(collider);
			collider = nullptr;
		}
		if (!entity->haveComponent<RigidBody>())
		{
			entity->removeComponent<Private::PhysicsData>();
		}
	}

#ifdef EDITOR_ENABLED

	void Collider::editorCreate(void)
	{
		return;
	}

	void Collider::editorDelete(void)
	{
		return;
	}

	bool Collider::editorUpdate(void)
	{
		if (editorStruct == nullptr)
		{
			editorStruct = std::make_unique<EditorStruct>();
		}
		editorStruct->copyDatas(this);
		editorStruct->editorUpdate(this);
		return false;
	}

	void Collider::EditorStruct::copyDatas(Collider *ptr)
	{
		if (isChoosingMesh == false)
		{
			currentType = ptr->getColliderType();
			if (currentType == Physics::ColliderType::Mesh)
			{
				isConcave = ptr->isConcave();
			}
		}
	}

	void Collider::EditorStruct::editorUpdate(Collider *ptr)
	{
		// Used to display the collider editor
		static bool folderInitialized = false;
		static FileUtils::Folder colliderFolder = FileUtils::Folder();
		// ---
		bool hasChanged = false;

		if (folderInitialized == false)
		{
			FileUtils::CookedFileFilter filter;
			std::string assetsDirectory = ptr->entity->getScene()->getInstance<AGE::AssetsManager>()->getAssetsDirectory();
			colliderFolder.list(&filter, assetsDirectory);
			folderInitialized = true;
		}

		std::string colliderType[] = 
		{
			"Box",
			"Sphere",
			"Capsule",
			"Mesh"
		};
		for (int i = 0; i < sizeof(colliderType) / sizeof(colliderType[0]); ++i)
		{
			if (ImGui::RadioButton(colliderType[i].c_str(), i == (int)currentType))
			{
				currentType = (Physics::ColliderType)i;
				hasChanged = true;
			}
		}
		if (currentType == Physics::ColliderType::Mesh)
		{
			if (ptr->getColliderType() != Physics::ColliderType::Mesh)
			{
				isChoosingMesh = true;
			}
			if (isChoosingMesh == false)
			{
				if (ImGui::RadioButton("Convex", !isConcave))
				{
					ptr->setAsConvex();
					isConcave = false;
				}
				if (ImGui::RadioButton("Concave", isConcave))
				{
					ptr->setAsConcave();
					isConcave = true;
				}
			}

			std::list<std::string> phageInFolder;

			colliderFolder.update(
				std::function<bool(FileUtils::Folder*)>([&](FileUtils::Folder* folder) {
				phageInFolder.clear();
				return true;
			}),
				std::function<bool(FileUtils::Folder*)>([](FileUtils::Folder* folder) {
				return true;
			}),
				std::function<void(FileUtils::CookedFile*)>([&](FileUtils::CookedFile* file) {
				
				auto extension = FileUtils::GetExtension(file->getFileName());
				// Name of the file without extension and without _static or _dynamic
				std::string phageName;

				// If its a .sage file, we just need to remove the extension to set the collider
				if (extension == "sage")
				{
					phageName = FileUtils::RemoveExtension(file->getFileName());

					if (std::find(phageInFolder.begin(), phageInFolder.end(), phageName) == phageInFolder.end())
					{
						phageInFolder.push_back(phageName);
						if (ImGui::Button(phageName.c_str()))
						{
							std::string phagePath = FileUtils::RemoveExtension(file->getPath());
							std::string assetsDirectory = ptr->entity->getScene()->getInstance<AGE::AssetsManager>()->getAssetsDirectory();
							_meshPath = phagePath.substr(assetsDirectory.size(), std::string::npos);
							hasChanged = true;
						}
					}
				}
			}));
		}
		if (hasChanged)
		{
			if (currentType == Physics::ColliderType::Mesh)
			{
				if (_meshPath.empty() == false)
				{
					isChoosingMesh = false;
					ptr->reset();
					ptr->init(currentType, _meshPath);
				}
			}
			else
			{
				ptr->reset();
				ptr->init(currentType);
			}
		}
	}

#endif

	void Collider::_copyFrom(const ComponentBase *model)
	{
		init();
		auto m = (Collider*)model;
#ifdef EDITOR_ENABLED
		editorUpdate();
		if (editorStruct)
		{
			editorStruct->copyDatas(m);
		}
		Link *link = entity.getLinkPtr();
		auto p = scaleFromMat4(link->getGlobalTransform());
		//scale(p / getSize());
#endif
	}
}
