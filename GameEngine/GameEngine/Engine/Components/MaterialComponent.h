#ifndef  __MATERIAL_COMPONENT_H__
#define  __MATERIAL_COMPONENT_H__

#include "Components/Component.hh"
#include <Core/Material.hh>
#include <Core/Engine.hh>
#include "Utils/SmartPointer.hh"

namespace Component
{
	class MaterialComponent : public Component::ComponentBase<MaterialComponent>
	{
	private:
		SmartPointer<Material>             _material;

		MaterialComponent();
		MaterialComponent(MaterialComponent const &);
		MaterialComponent                  &operator=(MaterialComponent const &);
	public:
		MaterialComponent(Engine &engine, Handle &entity, std::string const &name);
		MaterialComponent(Engine &engine, Handle &entity, SmartPointer<Material> material);
		virtual                            ~MaterialComponent(void);

		bool                				setMaterial(std::string const &name);
		SmartPointer<Material>              getMaterial() const;
	};
};

#endif  //__MATERIAL_COMPONENT_H__