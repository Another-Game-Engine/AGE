#pragma once

#include <Components/Component.hh>

namespace AGE
{
	// Very basic FreeFly component for the moment.
	// It might contain more information in the future
	struct FreeFlyComponent : public ComponentBase
	{
		size_t notEmpty;

		void init(AScene *scene);
	};
}

