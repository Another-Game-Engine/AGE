#include "MaterialComponent.h"
#include "Utils/PubSub.hpp"

namespace Component
{
	ComponentMaterial::ComponentMaterial(Engine &engine, std::string const &name)
		: ComponentBase<ComponentMaterial>(engine, "MaterialCpt"),
		_material(nullptr)
	{
		_material = _engine.getInstance<Renderer>().getMaterialManager().getMaterial(name);
	}

	ComponentMaterial::ComponentMaterial(Engine &engine, SmartPointer<Material> material)
		: ComponentBase<ComponentMaterial>(engine, "MaterialCpt"),
		_material(material)
	{
	}

	ComponentMaterial::~ComponentMaterial(void)
	{}

	bool                				ComponentMaterial::setMaterial(std::string const &name)
	{
		SmartPointer<Material> old = _material;
		_material = _engine.getInstance<Renderer>().getMaterialManager().getMaterial(name);
		PubSub::pub(std::string("MaterialComponentChanged"), this->getFather(), old, _material);
		return (_material != nullptr);
	}

	SmartPointer<Material>              ComponentMaterial::getMaterial() const
	{
		return _material;
	}

}