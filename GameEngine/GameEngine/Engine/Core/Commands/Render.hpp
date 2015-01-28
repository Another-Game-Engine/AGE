#pragma once

#include <functional>
#include <Utils/Containers/Vector.hpp>

#include <Render/GeometryManagement/Painting/Painter.hh>

namespace AGE
{
	class Engine;
	struct DrawableCollection;

	namespace Commands
	{
		namespace Render
		{
			struct Flush{
				std::size_t aligned;
			};

			//TODO implement in render thread directly
			//struct RefreshInputs
			//{};

			struct CopyDrawLists
			{
				AGE::Vector<AGE::DrawableCollection> list;
				
				CopyDrawLists(AGE::Vector<AGE::DrawableCollection> &c)
					: list(std::move(c))
				{}
			};

			struct RenderDrawLists
			{
				std::function<void(AGE::DrawableCollection)> function;
				RenderDrawLists(std::function<void(AGE::DrawableCollection)> _function)
					: function(_function)
				{}
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