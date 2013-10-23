#ifndef		EMPTY_COMPONENT_HH_
#define		EMPTY_COMPONENT_HH_

#include "AComponent.hh"
#include "Utils/SmartPointer.hh"
#include "ResourceManager/SharedMesh.hh"

namespace Components
{
	class EmptyComponent : public AComponent
	{
	private:
		EmptyComponent(EmptyComponent const &);
		EmptyComponent	&operator=(EmptyComponent const &);

	public:
		EmptyComponent();
		virtual ~EmptyComponent(void);

		virtual void	start();
		virtual void	update();
		virtual void	stop();
	};

}

#endif //!EMPTY_COMPONENT_HH_