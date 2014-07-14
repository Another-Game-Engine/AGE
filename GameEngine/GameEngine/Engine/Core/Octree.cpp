#include "Octree.hpp"
#include <Core/AScene.hh>
#include <Components/MeshRenderer.hh>
#include <Core/EntityFilter.hpp>
#include <Components/CameraComponent.hpp>
#include <Utils/Frustum.hpp>

using namespace AGE;

Octree::USER_OBJECT_ID Octree::addElement(COMPONENT_ID componentType, const Entity &entity)
{
	Octree::USER_OBJECT_ID res = USER_OBJECT_ID(-1);
	if (!_freeUserObjects.empty())
	{
		res = _freeUserObjects.top();
		_freeUserObjects.pop();
	}
	else
	{
		res = _userObjectCounter++;
	}
	
	_commands.push(Command());
	_commands.back().entity = entity;
	_commands.back().commandType.set(CommandType::Create);
	_commands.back().id = res;
	_commands.back().componentType = componentType;
	return res;
}

void Octree::removeElement(Octree::USER_OBJECT_ID id)
{
	_freeUserObjects.push(id);
	_commands.push(Command());
	_commands.back().id = id;
	_commands.back().commandType.set(CommandType::Delete);
	assert(id != (std::size_t)(-1));
}

void Octree::setPosition(const glm::vec3 &v, Octree::USER_OBJECT_ID id)
{
	_commands.push(Command());
	_commands.back().id = id;
	_commands.back().position = v;
	_commands.back().commandType.set(CommandType::Position);
}
void Octree::setOrientation(const glm::quat &v, Octree::USER_OBJECT_ID id)
{
	_commands.push(Command());
	_commands.back().id = id;
	_commands.back().orientation = v;
	_commands.back().commandType.set(CommandType::Orientation);
}

void Octree::setScale(const glm::vec3 &v, Octree::USER_OBJECT_ID id)
{
	_commands.push(Command());
	_commands.back().id = id;
	_commands.back().scale = v;
	_commands.back().commandType.set(CommandType::Scale);
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
	, const std::vector<gl::Key<gl::Vertices>> &glvertices
	, const std::vector<gl::Key<gl::Indices>> &glindices
	, const std::vector<BoundingInfos> &boundings)
{
	_commands.push(Command());
	_commands.back().id = id;
	_commands.back().glindices = glindices;
	_commands.back().glvertices = glvertices;
	_commands.back().boundings = boundings;
	_commands.back().commandType.set(CommandType::Geometry);
}

//-----------------------------------------------------------------

Octree::CULLABLE_ID Octree::addCullableObject(Octree::USER_OBJECT_ID uid)
{
	CULLABLE_ID res = CULLABLE_ID(-1);
	CullableObject *co = nullptr;
	if (!_freeCullableObjects.empty())
	{
		res = _freeCullableObjects.top();
		_freeCullableObjects.pop();
		co = &(_cullableObjects[res]);
	}
	else
	{
		res = _cullableObjects.size();
		_cullableObjects.emplace_back(CullableObject());
		co = &(_cullableObjects.back());
	}
	co->userObjectId = uid;
	co->id = res;
	co->active = true;
	return res;
}

void Octree::removeCullableObject(CULLABLE_ID id)
{
	_freeCullableObjects.push(id);
	_cullableObjects[id].active = false;
	assert(id != (std::size_t)(-1));
}

void Octree::update()
{
	while (!_commands.empty())
	{
		//process command
		auto &command = _commands.front();

		if (command.commandType.at(CommandType::Create))
		{
			UserObject *ue = nullptr;
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
			ue->componentType = command.componentType;
			ue->entity = command.entity;
			ue->active = true;
		}
		else if (command.commandType.at(CommandType::Delete))
		{
			auto &ue = _userObjects[command.id];
			auto debugId = ue.entity.getId();
			for (auto &e : ue.collection)
			{
				removeCullableObject(e);
			}
			ue.collection.clear();
			ue.active = false;
		}
		else if (command.commandType.at(CommandType::Geometry))
		{
			auto &ue = _userObjects[command.id];
			for (auto &e : ue.collection)
			{
				removeCullableObject(e);
			}
			ue.collection.clear();
			for (std::size_t i = 0; i < command.glindices.size(); ++i)
			{
				auto id = addCullableObject(command.id);
				_cullableObjects[id].bounding = command.boundings[i];
				_cullableObjects[id].glvertices = command.glvertices[i];
				_cullableObjects[id].glindices = command.glindices[i];
				_cullableObjects[id].position = ue.position;
				_cullableObjects[id].orientation = ue.orientation;
				_cullableObjects[id].scale = ue.scale;
			}
		}
		else if (command.commandType.at(CommandType::Position))
		{
			auto &ue = _userObjects[command.id];
			ue.position = command.position;
			for (auto &e : ue.collection)
			{
				_cullableObjects[e].position = ue.position;
			}
		}
		else if (command.commandType.at(CommandType::Scale))
		{
			auto &ue = _userObjects[command.id];
			ue.scale = command.scale;
			for (auto &e : ue.collection)
			{
				_cullableObjects[e].scale = ue.scale;
			}
		}
		else if (command.commandType.at(CommandType::Orientation))
		{
			auto &ue = _userObjects[command.id];
			ue.orientation = command.orientation;
			for (auto &e : ue.collection)
			{
				_cullableObjects[e].orientation = ue.orientation;
			}
		}
		_commands.pop();
	}

	static EntityFilter *filter = nullptr;
	if (!filter)
	{
		filter = new EntityFilter(std::move(scene));
		filter->requireComponent<Component::CameraComponent>();
	}

	if (filter->getCollection().size() == 0)
		return;
	auto cameraEntity = *(filter->getCollection().begin());
	auto camera = scene.lock()->getComponent<Component::CameraComponent>(cameraEntity);

	Frustum frustum;
	static float rot = 0.0f;
	rot += 1.0f;
	auto m = camera->projection * camera->lookAtTransform;
	frustum.setMatrix(m, true);
	std::uint64_t drawed = 0;
	std::uint64_t total = 0;

	//TMP
	for (auto &e : _userObjects)
	{
		if (e.active)
		{
			auto cpt = scene.lock()->getComponent(e.entity, e.componentType);
			if (cpt)
			{
				auto c = dynamic_cast<AGE::ComponentBehavior::Cullable*>(cpt);
				c->draw = true;
				++drawed;
			}
		}
	}
	//!TMP

	for (auto &e : _cullableObjects)
	{
		if (e.active)
			++total;
		if (e.active && frustum.pointIn(e.position) == true)
		{
			//std::cout << e.entity.getId() << "  ";
			auto &uo = _userObjects[e.userObjectId];
			auto cpt = scene.lock()->getComponent(uo.entity, uo.componentType);
			if (cpt)
			{
				auto c = dynamic_cast<AGE::ComponentBehavior::Cullable*>(cpt);
				c->draw = true;
				++drawed;
			}
			else
			{
				assert(false);
			}
		}
	}
//	std::cout << "Drawed : " << drawed << " / " << total << std::endl;
}