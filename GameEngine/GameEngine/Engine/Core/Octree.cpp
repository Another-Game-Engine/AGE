#include "Octree.hpp"
#include <Core/AScene.hh>
#include <Components/MeshRenderer.hh>
#include <Core/EntityFilter.hpp>
#include <Components/CameraComponent.hpp>
#include <Utils/Frustum.hpp>
#include <thread>
#include <chrono>

namespace AGE
{
	Octree::Octree()
	{
		// init in main thread
		_isRunning = false;
		_mainThreadDrawList = AGE::Vector<DrawableCollection>();

		// launch thread
		_thread = new std::thread(&Octree::_run, std::ref(*this));
	}

	Octree::~Octree(void)
	{
		_isRunning = false;
		_hasSomeWork.notify_one();
		_thread->join();
		delete _thread;
	}

	void Octree::_run()
	{
		_isRunning = true;
		_octreeDrawList = AGE::Vector<DrawableCollection>();
		while (_isRunning)
		{
			_update();
		}
	}


	const OctreeKey &Octree::addCullableElement()
	{
		OctreeKey res;
		res.type = OctreeKey::Type::Cullable;
		if (!_freeUserObjects.empty())
		{
			res.id = _freeUserObjects.front();
			_freeUserObjects.pop();
		}
		else
		{
			res.id = _userObjectCounter++;
		}

		auto c = _mainThreadCommands.push<OctreeCommand::CreateDrawable>();
		c->key = res;

		return res;
	}

	void Octree::removeElement(const OctreeKey &key)
	{
		assert(!key.invalid());
		switch (key.type)
		{
		case(OctreeKey::Type::Camera) :
		{
										  _freeCameraObjects.push(key.id);
										  auto c = _mainThreadCommands.push<OctreeCommand::DeleteCamera>();
										  c->key = key;
		}
									  break;
		case(OctreeKey::Type::Cullable) :
		{
											_freeUserObjects.push(key.id);
											auto c = _mainThreadCommands.push<OctreeCommand::DeleteDrawable>();
											c->key = key;
		}
										break;
		default:
			break;
		}
	}

	const OctreeKey &Octree::addCameraElement()
	{
		OctreeKey res;
		res.type = OctreeKey::Type::Camera;
		if (!_freeCameraObjects.empty())
		{
			res.id = _freeCameraObjects.front();
			_freeCameraObjects.pop();
		}
		else
		{
			res.id = _cameraCounter++;
		}

		_mainThreadCommands.push<OctreeCommand::CreateCamera>()->key = res;

		return res;
	}

	void Octree::setPosition(const glm::vec3 &v, const OctreeKey &id)
	{
		auto c = _mainThreadCommands.push<OctreeCommand::Position>();
		c->key = id;
		c->position = v;
	}
	void Octree::setOrientation(const glm::quat &v, const OctreeKey &id)
	{
		auto c = _mainThreadCommands.push<OctreeCommand::Orientation>();
		c->key = id;
		c->orientation = v;
	}

	void Octree::setScale(const glm::vec3 &v, const OctreeKey &id)
	{
		auto c = _mainThreadCommands.push<OctreeCommand::Scale>();
		c->key = id;
		c->scale = v;
	}

	void Octree::setCameraInfos(const OctreeKey &id
		, const glm::mat4 &projection)
	{
		auto c = _mainThreadCommands.push<OctreeCommand::CameraInfos>();
		c->key = id;
		c->projection = projection;
	}

	void Octree::setPosition(const glm::vec3 &v, const std::array<OctreeKey, MAX_CPT_NUMBER> &ids)
	{
		for (auto &e : ids)
			setPosition(v, e);
	}

	void Octree::setOrientation(const glm::quat &v, const std::array<OctreeKey, MAX_CPT_NUMBER> &ids)
	{
		for (auto &e : ids)
			setOrientation(v, e);
	}

	void Octree::setScale(const glm::vec3 &v, const std::array<OctreeKey, MAX_CPT_NUMBER> &ids)
	{
		for (auto &e : ids)
			setScale(v, e);
	}

	void Octree::updateGeometry(const OctreeKey &key
		, const AGE::Vector<AGE::SubMeshInstance> &meshs
		, const AGE::Vector<AGE::MaterialInstance> &materials)
	{
		assert(!key.invalid() || key.type != OctreeKey::Type::Cullable);
		auto c = _mainThreadCommands.push<OctreeCommand::Geometry>();
		c->key = key;
		c->submeshInstances = meshs;
		c->materialInstances = materials;
	}

	//-----------------------------------------------------------------

	Octree::DRAWABLE_ID Octree::addDrawableObject(Octree::USER_OBJECT_ID uid)
	{
		DRAWABLE_ID res = DRAWABLE_ID(-1);
		CullableObject *co = nullptr;
		if (!_freeCullableObjects.empty())
		{
			res = _freeCullableObjects.front();
			_freeCullableObjects.pop();
			co = &(_cullableObjects[res]);
		}
		else
		{
			res = _cullableObjects.size();
			_cullableObjects.emplace_back(CullableObject());
			co = &(_cullableObjects.back());
		}
		co->id = res;
		co->active = true;
		return res;
	}

	void Octree::removeDrawableObject(DRAWABLE_ID id)
	{
		_freeCullableObjects.push(id);
		_cullableObjects[id].active = false;
		assert(id != (std::size_t)(-1));
	}

	AGE::Vector<DrawableCollection> &Octree::getDrawableList()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_mainThreadDrawList = std::move(_octreeDrawList);
		std::swap(_mainThreadCommands, _octreeCommands);
//		_octreeCommands = std::move(_mainThreadCommands);
		_mainThreadCommands.clear();
		lock.unlock();
		_hasSomeWork.notify_one();
		return _mainThreadDrawList;
	}


	void Octree::_update()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_hasSomeWork.wait(lock);
		if (!_isRunning)
			return;

		while (!_octreeCommands.isEmpty())
		{
			//process command
			auto commandPtr = _octreeCommands.pop();

			UserObject *uo = nullptr;
			CameraObject *co = nullptr;
			if (commandPtr->uid == OctreeCommand::CreateDrawable::getId())
			{
				auto command = (OctreeCommand::CreateDrawable*)(commandPtr);
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
				break;
			}
			else if (commandPtr->uid == OctreeCommand::DeleteDrawable::getId())
			{
				auto command = (OctreeCommand::DeleteDrawable*)(commandPtr);
				uo = &_userObjects[command->key.id];
				for (auto &e : uo->drawableCollection)
				{
					removeDrawableObject(e);
				}
				uo->drawableCollection.clear();
				uo->active = false;
			}

			else if (commandPtr->uid == OctreeCommand::CreateCamera::getId())
			{
				auto command = (OctreeCommand::CreateCamera*)(commandPtr);
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
			else if (commandPtr->uid == OctreeCommand::DeleteCamera::getId())
			{
				auto command = (OctreeCommand::DeleteCamera*)(commandPtr);
				co = &_cameraObjects[command->key.id];
				co->active = false;
			}
			else if (commandPtr->uid == OctreeCommand::Geometry::getId())
			{
				auto command = (OctreeCommand::Geometry*)(commandPtr);
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
			else if (commandPtr->uid == OctreeCommand::Position::getId())
			{
				auto command = (OctreeCommand::Position*)(commandPtr);
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
			else if (commandPtr->uid == OctreeCommand::Scale::getId())
			{
				auto command = (OctreeCommand::Scale*)(commandPtr);
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
			else if (commandPtr->uid == OctreeCommand::Orientation::getId())
			{
				auto command = (OctreeCommand::Orientation*)(commandPtr);
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
			else if (commandPtr->uid == OctreeCommand::CameraInfos::getId())
			{
				auto command = (OctreeCommand::CameraInfos*)(commandPtr);
					co = &_cameraObjects[command->key.id];
					co->hasMoved = true;
					co->projection = command->projection;
			}
			_octreeCommands.pop();
		}

		_octreeDrawList.clear();

		static std::size_t cameraCounter = 0; cameraCounter = 0;

		for (auto &camera : _cameraObjects)
		{
			if (!camera.active)
				continue;
			Frustum frustum;
			auto transformation = glm::scale(glm::translate(glm::mat4(1), camera.position) * glm::toMat4(camera.orientation), camera.scale);
			frustum.setMatrix(camera.projection * transformation, true);

			_octreeDrawList.emplace_back();
			auto &drawList = _octreeDrawList.back();

			drawList.drawables.clear();

			drawList.transformation = transformation;
			drawList.projection = camera.projection;

			std::size_t drawed = 0; std::size_t total = 0;

			for (auto &e : _cullableObjects)
			{
				if (e.active)
					++total;
				else
					continue;
				if (frustum.pointIn(e.position) == true)
				{
					if (e.hasMoved)
					{
						e.transformation = glm::scale(glm::translate(glm::mat4(1), e.position) * glm::toMat4(e.orientation), e.scale);
						e.hasMoved = false;
					}
					drawList.drawables.emplace_back(e.mesh, e.material, e.transformation);
					++drawed;
				}
			}
			//std::cout << "Camera n[" << cameraCounter << "] : " << drawed << " / " << total << std::endl;
			++cameraCounter;
		}
		lock.unlock();
		//std::unique_lock<std::mutex> lock2(_mutex);
		//lock2.lock();
		//std::swap(_octreeDrawList, _mainThreadDrawList);
		//lock2.unlock();
	}
}