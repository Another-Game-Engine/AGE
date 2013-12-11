#ifndef  __MATERIAL_MANAGER_HH__
# define __MATERIAL_MANAGER_HH__

#include <map>
#include <string>
#include "Utils/SmartPointer.hh"
#include "Material.hh"
#include "OpenGL/Framebuffer.hh"

class Shader;
namespace Component
{
	class MeshRenderer;
};

class MaterialManager
{
public:
private:
	std::map<std::string, SmartPointer<Material> > _materials;
public:
	MaterialManager();
	~MaterialManager();
	SmartPointer<Material> getMaterial(const std::string &name);
	SmartPointer<Material> createMaterial(const std::string &name);
	std::map<std::string, SmartPointer<Material> > &getMaterialList();
private:
	MaterialManager(const MaterialManager &o);
	MaterialManager &operator=(const MaterialManager &o);
};

#endif //!__MATERIAL_MANAGER_HH__