#ifndef  __MATERIAL_MANAGER_HH__
# define __MATERIAL_MANAGER_HH__

#include <map>
#include <set>
#include "Utils/SmartPointer.hh"

class Material;
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
		std::set<std::string> preShaders;
		unsigned int weight;
		std::set<SmartPointer<Components::MeshRenderer> > objects;
		unsigned int id;

	};

	std::multimap<unsigned int, SmartPointer<ShaderHolder> > _shaders;

public:
	MaterialManager();
	~MaterialManager();
	SmartPointer<Material> getMaterial(const std::string &name);
	SmartPointer<Material> createMaterial(const std::string &name);
	void compile();
private:
	MaterialManager(const MaterialManager &o);
	MaterialManager &operator=(const MaterialManager &o);
};

#endif //!__MATERIAL_MANAGER_HH__