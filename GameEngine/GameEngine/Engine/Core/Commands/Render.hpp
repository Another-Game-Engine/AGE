#pragma once

#include <glm/glm.hpp>
#include <TMQ/message.hpp>
#include <Utils/Containers/Vector.hpp>

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
		};
	}
}