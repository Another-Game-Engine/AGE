#include "Octree.hpp"
#include <Core/AScene.hh>
#include <Components/MeshRenderer.hh>
#include <Core/EntityFilter.hpp>
#include <Components/CameraComponent.hpp>
#include <Utils/Frustum.hpp>

namespace AGE
{
	Octree::Octree()
	{
		_octreeCommands = &_commandsBuffer[0];
		_mainThreadCommands = &_commandsBuffer[1];
		_octreeDrawList = &_drawLists[0];
		_mainThreadDrawList = &_drawLists[1];
	}

	Octree::~Octree(void)
	{
	}


	Octree::USER_OBJECT_ID Octree::addElement()
	{
		Octree::USER_OBJECT_ID res = USER_OBJECT_ID(-1);
		if (!_freeUserObjects.empty())
		{
			res = _freeUserObjects.front();
			_freeUserObjects.pop();
		}
		else
		{
			res = _userObjectCounter++;
		}

		_mainThreadCommands->emplace(res, CommandType::CreateDrawable);
		return res;
	}

	void Octree::removeElement(Octree::USER_OBJECT_ID id)
	{
		_freeUserObjects.push(id);
		_mainThreadCommands->emplace(id, CommandType::DeleteDrawable);
		assert(id != (std::size_t)(-1));
	}

	Octree::USER_OBJECT_ID Octree::addCamera()
	{
		Octree::USER_OBJECT_ID res = USER_OBJECT_ID(-1);
		if (!_freeCameraObjects.empty())
		{
			res = _freeCameraObjects.front();
			_freeCameraObjects.pop();
		}
		else
		{
			res = _cameraCounter++;
		}

		_mainThreadCommands->emplace(res, CommandType::CreateCamera);
		return res;
	}

	void Octree::removeCamera(Octree::USER_OBJECT_ID id)
	{
		_freeCameraObjects.push(id);
		_mainThreadCommands->emplace(id, CommandType::DeleteCamera);
		assert(id != (std::size_t)(-1));
	}

	void Octree::setPosition(const glm::vec3 &v, Octree::USER_OBJECT_ID id)
	{
		_mainThreadCommands->emplace(id, v, CommandType::Position);
	}
	void Octree::setOrientation(const glm::quat &v, Octree::USER_OBJECT_ID id)
	{
		_mainThreadCommands->emplace(id, v, CommandType::Orientation);
	}

	void Octree::setScale(const glm::vec3 &v, Octree::USER_OBJECT_ID id)
	{
		_mainThreadCommands->emplace(id, v, CommandType::Scale);
	}

	void Octree::setCameraInfos(USER_OBJECT_ID id
		, const glm::mat4 &projection)
	{
		_mainThreadCommands->emplace(id, projection, CommandType::CameraInfos);
	}

	void Octree::setPosition(const glm::vec3 &v, const std::array<Octree::USER_OBJECT_ID, MAX_CPT_NUMBER> &ids)
	{
		for (auto &e : ids)
			setPosition(v, e);
	}

	void Octree::setOrientation(const glm::quat &v, const std::array<Octree::USER_OBJECT_ID, MAX_CPT_NUMBER> &ids)
	{
		for (auto &e : ids)
			setOrientation(v, e);
	}

	void Octree::setScale(const glm::vec3 &v, const std::array<Octree::USER_OBJECT_ID, MAX_CPT_NUMBER> &ids)
	{
		for (auto &e : ids)
			setScale(v, e);
	}

	void Octree::updateGeometry(USER_OBJECT_ID id
		, const std::vector<AGE::SubMeshInstance> &meshs
		, const std::vector<AGE::MaterialInstance> &materials)
	{
		_mainThreadCommands->emplace(id, meshs, materials, CommandType::Geometry);
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

	void Octree::update()
	{
		std::swap(_octreeCommands, _mainThreadCommands);
		std::swap(_octreeDrawList, _mainThreadDrawList);
		while (!_octreeCommands->empty())
		{
			//process command
			auto &command = _octreeCommands->front();

			UserObject *uo = nullptr;
			CameraObject *co = nullptr;
			switch (command.commandType)
			{
			case (CommandType::CreateDrawable) :

				if (command.id >= _userObjects.size())
				{
					_userObjects.push_back(UserObject());
					uo = &_userObjects.back();
				}
				else
				{
					uo = &_userObjects[command.id];
				}
				uo->id = command.id;
				uo->active = true;
				break;

			case (CommandType::DeleteDrawable) :

				uo = &_userObjects[command.id];
				for (auto &e : uo->drawableCollection)
				{
					removeDrawableObject(e);
				}
				uo->drawableCollection.clear();
				uo->active = false;
				break;

			case (CommandType::CreateCamera) :

				if (command.id >= _cameraObjects.size())
				{
					_cameraObjects.push_back(CameraObject());
					co = &_cameraObjects.back();
				}
				else
				{
					co = &_cameraObjects[command.id];
				}
				co->id = command.id;
				co->active = true;
				break;

			case (CommandType::DeleteCamera) :

				co = &_cameraObjects[command.id];
				co->active = false;
				break;

			case (CommandType::Geometry) :

				uo = &_userObjects[command.id];
				assert(uo->active != false);
				for (auto &e : uo->drawableCollection)
				{
					removeDrawableObject(e);
				}
				uo->drawableCollection.clear();
				for (std::size_t i = 0; i < command.submeshInstances.size(); ++i)
				{
					auto id = addDrawableObject(command.id);
					uo->drawableCollection.push_back(id);
					_cullableObjects[id].mesh = command.submeshInstances[i];
					_cullableObjects[id].position = uo->position;
					_cullableObjects[id].orientation = uo->orientation;
					_cullableObjects[id].scale = uo->scale;
				}
				break;
			case (CommandType::Position) :

				uo = &_userObjects[command.id];
				uo->position = command.position;
				for (auto &e : uo->drawableCollection)
				{
					_cullableObjects[e].position = uo->position;
					_cullableObjects[e].hasMoved = true;
				}
				break;

			case (CommandType::Scale) :

				uo = &_userObjects[command.id];
				uo->scale = command.scale;
				for (auto &e : uo->drawableCollection)
				{
					_cullableObjects[e].scale = uo->scale;
					_cullableObjects[e].hasMoved = true;
				}
				break;

			case (CommandType::Orientation) :

				uo = &_userObjects[command.id];
				uo->orientation = command.orientation;
				for (auto &e : uo->drawableCollection)
				{
					_cullableObjects[e].orientation = uo->orientation;
					_cullableObjects[e].hasMoved = true;
				}
				break;
			case (CommandType::CameraInfos) :

				co = &_cameraObjects[command.id];
				co->hasMoved = true;
				co->projection = command.projection;
				break;
			default:
				break;
			}
			_octreeCommands->pop();
		}

		_octreeDrawList->clear();

		static std::size_t cameraCounter = 0; cameraCounter = 0;

		for (auto &camera : _cameraObjects)
		{
			if (!camera.active)
				continue;
			Frustum frustum;
			auto transformation = glm::translate(glm::mat4(1), glm::vec3(0, 0, -10)); /*glm::scale(glm::translate(glm::mat4(1), camera.position) * glm::toMat4(camera.orientation), camera.scale) * */;
			frustum.setMatrix(camera.projection, true);

			_octreeDrawList->push(DrawableCollection());
			auto &drawList = _octreeDrawList->back();
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
				if (/*frustum.pointIn(e.position) ==*/ true)
				{
					if (e.hasMoved)
					{
						e.transformation = glm::scale(glm::translate(glm::mat4(1), e.position) * glm::toMat4(e.orientation), e.scale);
						e.hasMoved = false;
					}
					drawList.drawables.emplace(e.mesh, e.material, e.transformation);
					++drawed;
				}
			}
			//std::cout << "Camera n[" << cameraCounter << "] : " << drawed << " / " << total << std::endl;
			++cameraCounter;
		}
	}
}