#ifndef		EMPTY_COMPONENT_HH_
#define		EMPTY_COMPONENT_HH_

#include "Component.hh"
#include "Utils/SmartPointer.hh"
#include "ResourceManager/SharedMesh.hh"

class Engine;

namespace Component
{
	class EmptyComponent : public ComponentBase<EmptyComponent>
	{
	private:
		EmptyComponent(EmptyComponent const &);
		EmptyComponent	&operator=(EmptyComponent const &);

	public:
		EmptyComponent(Engine &engine, Handle &entity);
		virtual ~EmptyComponent(void);
	};

}

#endif //!EMPTY_COMPONENT_HH_