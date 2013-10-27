#ifndef  __MATERIAL_PASS_HH__
# define __MATERIAL_PASS_HH__

#include <string>
#include <list>

#include "Components/MeshRenderer.hh"

static				int uniqueId()
{
  static int			id = 0;
  return id++;
}

class MaterialPass
{
public:
private:
	// todo add a framebuffer index or pointer
	std::string _shader;
	std::list<Components::MeshRenderer*> _objects;
public:
	MaterialPass();
	~MaterialPass();
	static unsigned int	getId()
	{
		static unsigned int		 id = uniqueId();
		return id;
	}
private:
	MaterialPass(const MaterialPass &o);
	MaterialPass &operator=(MaterialPass &o);
};

#endif  //!__MATERIAL_PASS_HH__