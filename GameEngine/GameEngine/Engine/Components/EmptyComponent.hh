#ifndef		EMPTY_COMPONENT_HH_
#define		EMPTY_COMPONENT_HH_

#include "Component.hpp"
#include "Utils/SmartPointer.hh"
#include "ResourceManager/SharedMesh.hh"

namespace Component
{
	class EmptyComponent : public ComponentBase<EmptyComponent>
	{
	private:
		EmptyComponent(EmptyComponent const &);
		EmptyComponent	&operator=(EmptyComponent const &);

	public:
		EmptyComponent();
		virtual ~EmptyComponent(void);
	};

}

#endif //!EMPTY_COMPONENT_HH_