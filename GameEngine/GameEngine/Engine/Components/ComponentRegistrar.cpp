#include <Components/ComponentRegistrar.hpp>
#include <Core/AScene.hh>

namespace AGE
{
	ComponentRegistrar::ComponentRegistrar()
	{}

	ComponentRegistrar::~ComponentRegistrar()
	{
		_creationFunctions.clear();
		_typeIds.clear();
	}

	const std::string &ComponentRegistrar::getComponentName(ComponentType type)
	{
		return _componentNames[type];
	}

	void ComponentRegistrar::serializeJson(ComponentBase *c, cereal::JSONOutputArchive &ar)
	{
		auto id = c->getType();
		auto find = _jsonSaveMap.find(id);
		assert(find != std::end(_jsonSaveMap));
		find->second(c, ar);
	}

	void ComponentRegistrar::loadJson(std::size_t componentHashId, Entity &e, cereal::JSONInputArchive &ar)
	{
		auto id = _typeIds[componentHashId];

		auto voidCpt = e.getScene()->allocateComponent(id);

		auto find = _jsonLoadMap.find(id);
		assert(find != std::end(_jsonLoadMap));
		find->second(voidCpt, ar);

		auto cpt = (AGE::ComponentBase*)voidCpt;

		cpt->_typeId = id;
		cpt->entity = e;
		cpt->postUnserialization(e.getScene());

		e.addComponentPtr(cpt);
	}

	std::size_t ComponentRegistrar::getSystemIdForAgeId(ComponentType id)
	{
		auto f = _ageTypeIds.find(id);
		if (f == std::end(_ageTypeIds))
			return -1;
		return f->second;
	}

}