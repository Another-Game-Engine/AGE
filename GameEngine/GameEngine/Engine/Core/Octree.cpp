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

		_mainThreadCommands->emplace(res, CommandType::Create);
		return res;
	}

	void Octree::removeElement(Octree::USER_OBJECT_ID id)
	{
		_freeUserObjects.push(id);
		_mainThreadCommands->emplace(id, CommandType::Delete);
		assert(id != (std::size_t)(-1));
	}

	Octree::CAMERA_ID Octree::addCamera()
	{
		return -1; // TODO
	}

	void Octree::removeCamera(CAMERA_ID id)
	{
		// TODO
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

			UserObject *ue = nullptr;
			switch (command.commandType)
			{
			case (CommandType::Create) :

				if (command.id >= _userObjects.size())
				{
					_userObjects.push_back(UserObject());
					ue = &_userObjects.back();
				}
				else
				{
					ue = &_userObjects[command.id];
				}
				ue->id = command.id;
				ue->active = true;
				break;

			case (CommandType::Delete) :

				ue = &_userObjects[command.id];
				for (auto &e : ue->drawableCollection)
				{
					removeDrawableObject(e);
				}
				ue->drawableCollection.clear();
				ue->active = false;
				break;

			case (CommandType::Geometry) :

				ue = &_userObjects[command.id];
				assert(ue->active != false);
				for (auto &e : ue->drawableCollection)
				{
					removeDrawableObject(e);
				}
				ue->drawableCollection.clear();
				for (std::size_t i = 0; i < command.submeshInstances.size(); ++i)
				{
					auto id = addDrawableObject(command.id);
					ue->drawableCollection.push_back(id);
					_cullableObjects[id].mesh = command.submeshInstances[i];
					_cullableObjects[id].position = ue->position;
					_cullableObjects[id].orientation = ue->orientation;
					_cullableObjects[id].scale = ue->scale;
				}
				break;
			case (CommandType::Position) :

				ue = &_userObjects[command.id];
				ue->position = command.position;
				for (auto &e : ue->drawableCollection)
				{
					_cullableObjects[e].position = ue->position;
					_cullableObjects[e].hasMoved = true;
				}
				break;

			case (CommandType::Scale) :

				ue = &_userObjects[command.id];
				ue->scale = command.scale;
				for (auto &e : ue->drawableCollection)
				{
					_cullableObjects[e].scale = ue->scale;
					_cullableObjects[e].hasMoved = true;
				}
				break;

			case (CommandType::Orientation) :

				ue = &_userObjects[command.id];
				ue->orientation = command.orientation;
				for (auto &e : ue->drawableCollection)
				{
					_cullableObjects[e].orientation = ue->orientation;
					_cullableObjects[e].hasMoved = true;
				}
				break;
			case (CommandType::CameraInfos) :

				ue = &_userObjects[command.id];
				break;
			default:
				break;
			}
			_octreeCommands->pop();
		}

		_octreeDrawList->clear();
		for (auto &camera : _cameraObjects)
		{
			if (!camera.active)
				continue;
			Frustum frustum;
			auto transformation = glm::scale(glm::translate(glm::mat4(1), camera.position) * glm::toMat4(camera.orientation), camera.scale);
			frustum.setMatrix(camera.projection * transformation, true);

			std::uint64_t drawed = 0;
			std::uint64_t total = 0;

			_octreeDrawList->push(DrawableCollection());
			auto &drawList = _octreeDrawList->back();
			drawList.drawables.clear();

			drawList.transformation = transformation;
			drawList.projection = camera.projection;

			for (auto &e : _cullableObjects)
			{
				if (e.active)
					++total;
				if (e.active && frustum.pointIn(e.position) == true)
				{
					if (e.hasMoved)
					{
						e.transformation = glm::scale(glm::translate(glm::mat4(1), e.position) * glm::toMat4(e.orientation), e.scale);
						e.hasMoved = false;
					}
					drawList.drawables.emplace(e.mesh, e.transformation);
				}
			}
		}
		//std::cout << "Drawed : " << drawed << " / " << total << std::endl;
	}
}