#pragma once

# include <array>
# include <unordered_map>
# include <glm/glm.hpp>
# include <iostream>
# include <string>
#include <Utils/OpenGL.hh>

static size_t const nbr_resources = 21;

extern const std::array<GLenum, nbr_resources> available_resources;

struct GlType
{
	uint8_t nbr_component;
	GLenum type;
	GLenum type_component;
	size_t size;
	std::string name;
	GlType(){}
	GlType(GLenum type, GLenum typeComponent, size_t size, uint8_t nbrComponent, std::string &&name) :
		nbr_component(nbrComponent),
		type(type),
		type_component(typeComponent),
		size(size),
		name(std::move(name))
	{}
	GlType(GlType const &copy) :
		nbr_component(copy.nbr_component),
		type(copy.type),
		type_component(copy.type_component),
		size(copy.size),
		name(copy.name)
	{}

	bool operator==(GLenum t) const
	{
		if (type == t) {
			return (true);
		}
		return (false);
	}

	bool operator!=(GLenum t) const
	{
		return (!(*this == t));
	}

	void print() const
	{
		std::cout << name << std::endl;
	}
};

extern std::unordered_map<GLenum, GlType> available_types;