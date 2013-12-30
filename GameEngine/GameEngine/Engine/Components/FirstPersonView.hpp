#ifndef    __FIRST_PERSON_VIEW_COMPONENT_HPP__
# define   __FIRST_PERSON_VIEW_COMPONENT_HPP__

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Component.hh"
#include <Utils/SmartPointer.hh>

namespace Component
{
	class FirstPersonView : public ComponentBase<FirstPersonView>
	{
	private:
		FirstPersonView(FirstPersonView const &);
		FirstPersonView	&operator=(FirstPersonView const &);
	public:
		FirstPersonView(AScene *scene, Handle &entity) :
			ComponentBase<FirstPersonView>(scene, entity, "FirstPersonView")
		{}
		virtual              ~FirstPersonView(void){}
		void init(){}
		virtual void reset(){}
	};
}

#endif     //__FIRST_PERSON_VIEW_COMPONENT_HPP__