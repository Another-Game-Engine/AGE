#include <Components/ComponentRegistrationManager.hpp>
#include <Core/AScene.hh>

namespace AGE
{
	ComponentRegistrationManager::ComponentRegistrationManager()
	{}

	ComponentRegistrationManager::~ComponentRegistrationManager()
	{
		_creationFunctions.clear();
		_typeIds.clear();
	}

	const std::string &ComponentRegistrationManager::getComponentName(ComponentType type)
	{
		return _componentNames[type];
	}

	void ComponentRegistrationManager::serializeJson(ComponentBase *c, cereal::JSONOutputArchive &ar)
	{
		auto id = c->getType();
		auto find = _jsonSaveMap.find(id);
		assert(find != std::end(_jsonSaveMap));
		find->second(c, ar);
	}

	void ComponentRegistrationManager::loadJson(std::size_t componentHashId, Entity &e, cereal::JSONInputArchive &ar)
	{
		AGE_ASSERT(_typeIds.find(componentHashId) != std::end(_typeIds) && "Component type has not been registered. Use REGISTER_COMPONENT_TYPE");
		auto id = _typeIds[componentHashId];

		auto voidCpt = e.getScene()->allocateComponent(id);

		auto find = _jsonLoadMap.find(id);
		assert(find != std::end(_jsonLoadMap));
		find->second(voidCpt, ar);

		auto cpt = (AGE::ComponentBase*)voidCpt;

		cpt->_typeId = id;
		cpt->entity = e;
		cpt->postUnserialization();

		e.addComponentPtr(cpt);
	}

	ComponentBase *ComponentRegistrationManager::copyComponent(ComponentBase *src, AScene *scene)
	{
		AGE_ASSERT(scene != nullptr);
		AGE_ASSERT(src != nullptr);

		auto id = src->getType();
		auto find = _copyMap.find(id);
		assert(find != std::end(_copyMap));
		auto voidCpt = scene->allocateComponent(id);
		find->second(voidCpt, src);
		return static_cast<ComponentBase*>(voidCpt);
	}

	std::size_t ComponentRegistrationManager::getSystemIdForAgeId(ComponentType id)
	{
		auto f = _ageTypeIds.find(id);
		if (f == std::end(_ageTypeIds))
			return -1;
		return f->second;
	}

}