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
	struct MeshRenderer;
};

class MaterialManager
{
private:
	Engine &_engine;
	std::map<std::string, SmartPointer<Material> > _materials;
public:
	MaterialManager(Engine &engine);
	~MaterialManager();
	SmartPointer<Material> getMaterial(const std::string &name);
	SmartPointer<Material> createMaterial(const std::string &name);
	std::map<std::string, SmartPointer<Material> > &getMaterialList();
	inline Engine &getEngine(){ return _engine; }
private:
	MaterialManager(const MaterialManager &o);
	MaterialManager &operator=(const MaterialManager &o);
};

#endif //!__MATERIAL_MANAGER_HH__