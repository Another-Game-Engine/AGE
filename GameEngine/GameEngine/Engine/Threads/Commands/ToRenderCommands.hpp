#pragma once

#include <functional>
#include <Utils/Containers/Vector.hpp>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <AssetManagement/Instance/MaterialInstance.hh>

namespace AGE
{
	class Engine;

	namespace Commands
	{
		namespace ToRender
		{
			struct Flush{
				std::size_t aligned;
			};

			struct CopyDrawLists
			{
				AGE::Vector<AGE::RenderCamera> list;
				
				CopyDrawLists(AGE::Vector<AGE::RenderCamera> &c)
					: list(std::move(c))
				{}
			};

			struct RenderDrawLists
			{
				int lol;
				RenderDrawLists() { }
			};

			struct DrawTestTriangle
			{
				std::shared_ptr<Painter> painter;
				AGE::Vector<Key<Vertices>> meshes;
				std::shared_ptr<Program> program;

				DrawTestTriangle(std::shared_ptr<Painter> const &p_painter,
					AGE::Vector<Key<Vertices>> const &p_meshes,
					std::shared_ptr<Program> const &p_proram) :
					painter(p_painter),
					meshes(p_meshes),
					program(p_proram)
				{ }
			};
		};
	}
}