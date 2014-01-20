#ifndef    __FIRST_PERSON_VIEW_COMPONENT_HPP__
# define   __FIRST_PERSON_VIEW_COMPONENT_HPP__

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Component.hh"
#include <Utils/SmartPointer.hh>

namespace Component
{
	struct FirstPersonView : public ComponentBase<FirstPersonView>
	{
	public:
		FirstPersonView() :
			ComponentBase<FirstPersonView>()
		{}
		virtual              ~FirstPersonView(void){}
		void init(){}
		virtual void reset(){}
	private:
		FirstPersonView(FirstPersonView const &);
		FirstPersonView	&operator=(FirstPersonView const &);
	};
}

#endif     //__FIRST_PERSON_VIEW_COMPONENT_HPP__