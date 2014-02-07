#pragma once

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
	std::list<Component::MeshRenderer*> _objects;
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