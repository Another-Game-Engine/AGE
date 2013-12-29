#include "MaterialComponent.h"
#include "Utils/PubSub.hpp"
#include <Core/Renderer.hh>

namespace Component
{
	MaterialComponent::MaterialComponent(Engine &engine, Handle &entity)
		: ComponentBase<MaterialComponent>(engine, entity, "MaterialCpt"),
		_material(nullptr)
	{
	}

	MaterialComponent::~MaterialComponent(void)
	{}

	void MaterialComponent::init(std::string const &name)
	{
		_material = _engine.getInstance<Renderer>().getMaterialManager().getMaterial(name);
	}

	void MaterialComponent::init(SmartPointer<Material> material)
	{
		_material = material;
	}

	void MaterialComponent::reset()
	{
		_material = nullptr;
	}


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