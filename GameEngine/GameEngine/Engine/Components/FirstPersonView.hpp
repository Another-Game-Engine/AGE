#ifndef    __FIRST_PERSON_VIEW_COMPONENT_HPP__
# define   __FIRST_PERSON_VIEW_COMPONENT_HPP__

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Component.hh"

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

		//////
		////
		// Serialization

		template <typename Archive>
		Base *unserialize(Archive &ar, Entity e)
		{
			auto res = new FirstPersonView();
			res->setEntity(e);
			ar(*res);
			return res;
		}

		template <typename Archive>
		void serialize(Archive &ar)
		{
		}

		// !Serialization
		////
		//////

	private:
		FirstPersonView(FirstPersonView const &);
		FirstPersonView	&operator=(FirstPersonView const &);
	};
}

#endif     //__FIRST_PERSON_VIEW_COMPONENT_HPP__