#ifndef  __MATERIAL_MANAGER_HH__
# define __MATERIAL_MANAGER_HH__

#include <map>
#include <set>
#include <string>
#include "Utils/SmartPointer.hh"
#include "Material.hh"
#include "OpenGL/Framebuffer.hh"

class Shader;
namespace Components
{
	class MeshRenderer;
};

class MaterialManager
{
public:
private:
	std::map<std::string, SmartPointer<Material> > _materials;
	std::map<std::string, unsigned int> _shaderIds;

	struct ShaderHolder
	{
		std::set<unsigned int> preShaders;
		unsigned int weight;
		std::map<std::string, SmartPointer<Material> > materials;
		unsigned int id;
		std::string name;
		bool last;
	};

	std::multimap<unsigned int, SmartPointer<ShaderHolder> > _shaders;
	std::map<unsigned int, SmartPointer<ShaderHolder> > _tmpList;
public:
	MaterialManager();
	~MaterialManager();
	SmartPointer<Material> getMaterial(const std::string &name);
	SmartPointer<Material> createMaterial(const std::string &name);
	void compile(OpenGLTools::Framebuffer &fbo);
	inline std::multimap<unsigned int, SmartPointer<ShaderHolder> > &getShaderList() {return _shaders;}
private:
	MaterialManager(const MaterialManager &o);
	MaterialManager &operator=(const MaterialManager &o);
	SmartPointer<ShaderHolder> getShaderHolder(const std::string &name);
};

#endif //!__MATERIAL_MANAGER_HH__