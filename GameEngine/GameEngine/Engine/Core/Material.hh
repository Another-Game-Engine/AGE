#ifndef  __MATERIAL_HH__
# define __MATERIAL_HH__

#include <vector>
#include <set>

#include "Utils/SmartPointer.hh"

namespace Components
{
	class MeshRenderer;
};
class MaterialManager;

class Material
{
public:
private:
	MaterialManager *_manager;
	std::vector<std::string> _shaders;
	std::set<Components::MeshRenderer*> _meshs;
	std::string _name;
	// TODO ass an FB pointer for _lastFrameBuffer;
public:
	Material(MaterialManager *manager, const std::string &name);
	~Material();
	Material &pushShader(const std::string &shader);
	const std::vector<std::string> &getShaders() const;
	void addObject(Components::MeshRenderer *object);
	void removeObject(Components::MeshRenderer *object);
	inline const std::string &getName() {return _name;}
	inline std::set<Components::MeshRenderer*> &getObjects() {return _meshs;}
private:
	Material(const Material &o);
	Material &operator=(const Material &o);
};

#endif   //!__MATERIAL_HH__