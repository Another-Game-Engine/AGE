#ifndef  __MATERIAL_HH__
# define __MATERIAL_HH__

#include <list>

class MaterialPass;

class Material
{
public:
private:
	std::list<MaterialPass> _passes;
	// TODO ass an FB pointer for _lastFrameBuffer;
public:
	Material();
	~Material();
private:
	Material(const Material &o);
	Material &operator=(const Material &o);
};

#endif   //!__MATERIAL_HH__