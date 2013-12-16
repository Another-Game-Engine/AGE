#include "MaterialComponent.h"
#include "Utils/PubSub.hpp"
#include <Core/Renderer.hh>

namespace Component
{
	MaterialComponent::MaterialComponent(Engine &engine, Handle &entity, std::string const &name)
		: ComponentBase<MaterialComponent>(engine, entity, "MaterialCpt"),
		_material(nullptr)
	{
		_material = _engine.getInstance<Renderer>().getMaterialManager().getMaterial(name);
	}

	MaterialComponent::MaterialComponent(Engine &engine, Handle &entity, SmartPointer<Material> material)
		: ComponentBase<MaterialComponent>(engine, entity, "MaterialCpt"),
		_material(material)
	{
	}

	MaterialComponent::~MaterialComponent(void)
	{}

	bool                				MaterialComponent::setMaterial(std::string const &name)
	{
		SmartPointer<Material> old = _material;
		_material = _engine.getInstance<Renderer>().getMaterialManager().getMaterial(name);
		_engine.getInstance<PubSub::Manager>().pub(std::string("MaterialComponentChanged"), this->getEntity(), old, _material);
		return (_material != nullptr);
	}

	SmartPointer<Material>              MaterialComponent::getMaterial() const
	{
		return _material;
	}

}