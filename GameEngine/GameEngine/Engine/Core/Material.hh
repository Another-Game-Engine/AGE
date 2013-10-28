#ifndef  __MATERIAL_HH__
# define __MATERIAL_HH__

#include <vector>

class MaterialManager;

class Material
{
public:
private:
	MaterialManager *_manager;
	std::vector<std::string> _shaders;
	// TODO ass an FB pointer for _lastFrameBuffer;
public:
	Material(MaterialManager *manager);
	~Material();
	Material &pushShader(const std::string &shader);
	const std::vector<std::string> &getShaders() const;
private:
	Material(const Material &o);
	Material &operator=(const Material &o);
};

#endif   //!__MATERIAL_HH__