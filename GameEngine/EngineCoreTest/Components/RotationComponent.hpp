#pragma once

#include <Components/Component.hh>
#include <Utils/Serialization/VectorSerialization.hpp>

namespace AGE
{
	struct RotationComponent : public ComponentBase
	{
		RotationComponent();
		virtual ~RotationComponent(void);

		void init(const glm::vec3 &angles = glm::vec3(0));

		virtual void reset();

		//////
		////
		// Serialization

		template <typename Archive>
		void serialize(Archive &ar)
		{
			ar(cereal::make_nvp("Angles", _angles));
		}

		// !Serialization
		////
		//////

#ifdef EDITOR_ENABLED
		virtual void editorUpdate(AScene *scene);
#endif

		glm::vec3 _angles;
	};
}