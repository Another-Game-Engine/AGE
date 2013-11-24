#include "MaterialComponent.h"
#include "Utils/PubSub.hpp"

namespace Component
{
	ComponentMaterial::ComponentMaterial(std::string const &name)
		: _material(nullptr)
	{
		_material = GameEngine::instance()->renderer().getMaterialManager().getMaterial(name);
	}

	ComponentMaterial::ComponentMaterial(SmartPointer<Material> material)
		: _material(material)
	{
	}

	ComponentMaterial::~ComponentMaterial(void)
	{}

	bool                				ComponentMaterial::setMaterial(std::string const &name)
	{
		_material = GameEngine::instance()->renderer().getMaterialManager().getMaterial(name);
		PubSub::pub(std::string("MaterialComponentChanged"), this->getFather());
		return (_material != nullptr);
	}

	SmartPointer<Material>              ComponentMaterial::getMaterial() const
	{
		return _material;
	}

}