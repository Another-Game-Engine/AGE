#pragma once

#include <vector>
#include <map>
#include <OpenGL/Vertice.hh>

#include <Utils/DependenciesInjector.hpp>

namespace AGE
{

	template <int S>
	class Mesh
	{
	public:
		Mesh(Vertice<S> &buffer)
			: _buffer(buffer)
		{}

		~Mesh()
		{}

		Vertice<S> &_buffer;
	};
}