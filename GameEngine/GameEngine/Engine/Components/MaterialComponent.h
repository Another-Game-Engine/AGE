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
		MaterialComponent(AScene *scene, Entity &entity);
		void init(std::string const &name);
		void init(SmartPointer<Material> material);
		virtual void reset();
		virtual                            ~MaterialComponent(void);

		bool                				setMaterial(std::string const &name);
		SmartPointer<Material>              getMaterial() const;
	};
};

#endif  //__MATERIAL_COMPONENT_H__