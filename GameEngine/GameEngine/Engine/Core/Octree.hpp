#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Entities/EntityTypedef.hpp>

#include <Utils/Dependency.hpp>
#include <Utils/BoundingInfos.hpp>

#include <Entities/Entity.hh>

#include <OpenGL/Key.hh>
#include <OpenGL/Data.hh>

#include <stack>

#include <Utils/Containers/Queue.hpp>
#include <Utils/Containers/Vector.hpp>

#include <Geometry/Mesh.hpp>
#include <Geometry/Material.hpp>
#include <Core/Drawable.hh>

#include <Core/OctreeKey.hpp>

#include <tmq/doubleBuffered/templateDispatcher.hpp>
#include <tmq/doubleBuffered/queue.hpp>

#include <thread>
#include <condition_variable>
#include <atomic>

class AScene;

namespace AGE
{
	struct Drawable;

	namespace OctreeCommand
	{
		struct Position
		{
			Position(const OctreeKey &_key, const glm::vec3 &_position)
				: key(_key)
				, position(_position)
			{}
			OctreeKey key;
			glm::vec3 position;
		};

		struct Scale
		{
			Scale(const OctreeKey &_key, const glm::vec3 &_scale)
				: key(_key)
				, scale(_scale)
			{}
			OctreeKey key;
			glm::vec3 scale;
		};

		struct Orientation
		{
			Orientation(const OctreeKey &_key, const glm::quat &_orientation)
				: key(_key)
				, orientation(_orientation)
			{}
			OctreeKey key;
			glm::quat orientation;
		};

		struct Geometry
		{
			Geometry(const OctreeKey &_key
				, const AGE::Vector<SubMeshInstance> &_submeshInstances
				, const AGE::Vector<MaterialInstance> &_materialInstances)
				: key(_key)
				, submeshInstances(_submeshInstances)
				, materialInstances(_materialInstances)
			{}
			OctreeKey key;
			AGE::Vector<SubMeshInstance> submeshInstances;
			AGE::Vector<MaterialInstance> materialInstances;
		};

		struct CreateDrawable
		{
			CreateDrawable(const OctreeKey &_key)
				: key(_key)
			{}
			OctreeKey key;
		};

		struct DeleteDrawable
		{
			DeleteDrawable(const OctreeKey &_key)
				: key(_key)
			{}
			OctreeKey key;
		};

		struct CreateCamera
		{
			CreateCamera(const OctreeKey &_key)
				: key(_key)
			{}
			OctreeKey key;
		};

		struct DeleteCamera
		{
			DeleteCamera(const OctreeKey &_key)
				: key(_key)
			{}
			OctreeKey key;
		};

		struct CameraInfos
		{
			CameraInfos(const OctreeKey &_key, const glm::mat4 &_projection)
				: key(_key)
				, projection(_projection)
			{}
			OctreeKey key;
			glm::mat4 projection;
		};
	}

	class Octree : public Dependency<Octree>
	{
	public:
		typedef std::uint64_t USER_OBJECT_ID;

	private:
		typedef std::uint64_t DRAWABLE_ID;

	private:
		struct CullableObject
		{
			glm::vec3 position;
			glm::vec3 scale;
			glm::quat orientation;
			DRAWABLE_ID id;
			bool active;
			SubMeshInstance mesh;
			MaterialInstance material;
			bool hasMoved;
			BoundingInfos boundingInfo;
			glm::mat4 transformation;
		};

		struct CameraObject
		{
			OctreeKey key;
			glm::vec3 position;
			glm::vec3 scale;
			glm::quat orientation;
			bool active;
			bool hasMoved;
			glm::mat4 projection;
		};

		struct UserObject
		{
			AGE::Vector<DRAWABLE_ID> drawableCollection;
			USER_OBJECT_ID id;
			glm::vec3 position;
			glm::vec3 scale;
			glm::quat orientation;
			bool active;
		};


	private:
		//TMP
		std::weak_ptr<AScene> scene;
	public:
		void setScene(std::weak_ptr<AScene> _scene) { scene = _scene; }
	public:
		Octree();
		virtual ~Octree(void);
	private:
		AGE::Vector<UserObject> _userObjects;
		AGE::Queue<OctreeKey::OctreeObjectId> _freeUserObjects;
		AGE::Vector<CullableObject> _cullableObjects;
		AGE::Queue<OctreeKey::OctreeObjectId> _freeCullableObjects;
		AGE::Vector<CameraObject> _cameraObjects;
		AGE::Queue<OctreeKey::OctreeObjectId> _freeCameraObjects;
		std::size_t _userObjectCounter = 0;
		std::size_t _cameraCounter = 0;

		TMQ::Double::Queue _commandQueue;

		AGE::Vector<DrawableCollection> _octreeDrawList;
		AGE::Vector<DrawableCollection> _mainThreadDrawList;

	public:
		const OctreeKey &addCullableElement();
		const OctreeKey &addCameraElement();

		void removeElement(const OctreeKey &key);


		void setPosition(const glm::vec3 &v, const OctreeKey &key);
		void setOrientation(const glm::quat &v, const OctreeKey &key);
		void setScale(const glm::vec3 &v, const OctreeKey &key);

		void setPosition(const glm::vec3 &v, const std::array<OctreeKey, MAX_CPT_NUMBER> &ids);
		void setOrientation(const glm::quat &v, const std::array<OctreeKey, MAX_CPT_NUMBER> &ids);
		void setScale(const glm::vec3 &v, const std::array<OctreeKey, MAX_CPT_NUMBER> &ids);

		void updateGeometry(const OctreeKey &id
			, const AGE::Vector<AGE::SubMeshInstance> &meshs
			, const AGE::Vector<AGE::MaterialInstance> &materials);

		void setCameraInfos(const OctreeKey &id
			, const glm::mat4 &projection);

		AGE::Vector<DrawableCollection> &getDrawableList();

		//
		// END
	private:
		void _update();
		DRAWABLE_ID addDrawableObject(Octree::USER_OBJECT_ID uid);
		void removeDrawableObject(DRAWABLE_ID id);
		void _run();

		std::thread *_thread;
		std::atomic_bool _isRunning;

		/*void _executeCommand(OctreeCommand::CreateDrawable *command)
		{
		UserObject *uo = nullptr;

		if (command->key.id >= _userObjects.size())
		{
		_userObjects.push_back(UserObject());
		uo = &_userObjects.back();
		}
		else
		{
		uo = &_userObjects[command->key.id];
		}
		uo->id = command->key.id;
		uo->active = true;
		}

		void _executeCommand(OctreeCommand::DeleteDrawable *command)
		{
		UserObject *uo = nullptr;
		uo = &_userObjects[command->key.id];
		for (auto &e : uo->drawableCollection)
		{
		removeDrawableObject(e);
		}
		uo->drawableCollection.clear();
		uo->active = false;
		}

		void _executeCommand(OctreeCommand::CreateCamera *command)
		{
		CameraObject *co = nullptr;
		if (command->key.id >= _cameraObjects.size())
		{
		_cameraObjects.push_back(CameraObject());
		co = &_cameraObjects.back();
		}
		else
		{
		co = &_cameraObjects[command->key.id];
		}
		co->key.id = command->key.id;
		co->active = true;
		}

		void _executeCommand(OctreeCommand::DeleteCamera *command)
		{
		CameraObject *co = nullptr;
		co = &_cameraObjects[command->key.id];
		co->active = false;
		}

		void _executeCommand(OctreeCommand::Geometry *command)
		{
		UserObject *uo = nullptr;

		uo = &_userObjects[command->key.id];
		assert(uo->active != false);
		for (auto &e : uo->drawableCollection)
		{
		removeDrawableObject(e);
		}
		uo->drawableCollection.clear();
		for (std::size_t i = 0; i < command->submeshInstances.size(); ++i)
		{
		auto id = addDrawableObject(command->key.id);
		uo->drawableCollection.push_back(id);
		_cullableObjects[id].mesh = command->submeshInstances[i];
		_cullableObjects[id].material = command->materialInstances[i];
		_cullableObjects[id].position = uo->position;
		_cullableObjects[id].orientation = uo->orientation;
		_cullableObjects[id].scale = uo->scale;
		}
		}

		void _executeCommand(OctreeCommand::Position *command)
		{
		UserObject *uo = nullptr;
		CameraObject *co = nullptr;

		switch (command->key.type)
		{
		case(OctreeKey::Type::Camera) :
		co = &_cameraObjects[command->key.id];
		co->position = command->position;
		co->hasMoved = true;
		break;
		case(OctreeKey::Type::Cullable) :
		uo = &_userObjects[command->key.id];
		uo->position = command->position;
		for (auto &e : uo->drawableCollection)
		{
		_cullableObjects[e].position = uo->position;
		_cullableObjects[e].hasMoved = true;
		}
		break;
		default:
		break;
		}
		}

		void _executeCommand(OctreeCommand::Scale *command)
		{
		UserObject *uo = nullptr;
		CameraObject *co = nullptr;

		switch (command->key.type)
		{
		case(OctreeKey::Type::Camera) :
		co = &_cameraObjects[command->key.id];
		co->scale = command->scale;
		co->hasMoved = true;
		break;
		case(OctreeKey::Type::Cullable) :
		uo = &_userObjects[command->key.id];
		uo->scale = command->scale;
		for (auto &e : uo->drawableCollection)
		{
		_cullableObjects[e].scale = uo->scale;
		_cullableObjects[e].hasMoved = true;
		}
		break;
		default:
		break;
		}
		}

		void _executeCommand(OctreeCommand::Orientation *command)
		{
		UserObject *uo = nullptr;
		CameraObject *co = nullptr;

		switch (command->key.type)
		{
		case(OctreeKey::Type::Camera) :
		co = &_cameraObjects[command->key.id];
		co->orientation = command->orientation;
		co->hasMoved = true;
		break;
		case(OctreeKey::Type::Cullable) :
		uo = &_userObjects[command->key.id];
		uo->orientation = command->orientation;
		for (auto &e : uo->drawableCollection)
		{
		_cullableObjects[e].orientation = uo->orientation;
		_cullableObjects[e].hasMoved = true;
		}
		break;
		default:
		break;
		}
		}

		void _executeCommand(OctreeCommand::CameraInfos *command)
		{

		CameraObject *co = nullptr;

		co = &_cameraObjects[command->key.id];
		co->hasMoved = true;
		co->projection = command->projection;
		}*/
	};
}