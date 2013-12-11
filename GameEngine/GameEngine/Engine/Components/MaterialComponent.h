#ifndef  __MATERIAL_COMPONENT_H__
#define  __MATERIAL_COMPONENT_H__

#include "Components/Component.hpp"
#include <Core/Material.hh>
#include <Core/Engine.hh>
#include "Utils/SmartPointer.hh"

namespace Component
{
	class ComponentMaterial : public Component::ComponentBase<ComponentMaterial>
	{
	private:
		SmartPointer<Material>             _material;

		ComponentMaterial();
		ComponentMaterial(ComponentMaterial const &);
		ComponentMaterial                  &operator=(ComponentMaterial const &);
	public:
		ComponentMaterial(Engine &engine, std::string const &name);
		ComponentMaterial(Engine &engine, SmartPointer<Material> material);
		virtual                            ~ComponentMaterial(void);

		bool                				setMaterial(std::string const &name);
		SmartPointer<Material>              getMaterial() const;
	};
};

#endif  //__MATERIAL_COMPONENT_H__