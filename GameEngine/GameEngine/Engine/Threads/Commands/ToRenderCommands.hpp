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
				std::shared_ptr<RenderCameraListContainerHandle> listContainer;
				
				CopyDrawLists(std::shared_ptr<RenderCameraListContainerHandle> handle)
					: listContainer(handle)
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

			struct Draw2DLine
			{
				glm::vec2 start;
				glm::vec2 end;

				Draw2DLine(glm::vec2 _start, glm::vec2 _end) :
					start(_start),
					end(_end)
				{
				}
			};
		};
	}
}