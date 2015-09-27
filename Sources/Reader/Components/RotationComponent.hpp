#pragma once

#include <Components/Component.hh>
#include <Utils/Serialization/VectorSerialization.hpp>
#include <glm/glm.hpp>

namespace AGE
{
	struct RotationComponent : public ComponentBase
	{
		AGE_COMPONENT_UNIQUE_IDENTIFIER("AGE_CORE_RotationComponent");

		RotationComponent();
		virtual ~RotationComponent(void);
		RotationComponent &operator=(const RotationComponent &o) = delete;
		virtual void _copyFrom(const ComponentBase *model);

		void init(const glm::vec3 &angles = glm::vec3(0), float speed = 100.0f);

		virtual void reset();

		//////
		////
		// Serialization

		template <typename Archive>
		void serialize(Archive &ar, uint32_t version)
		{
			if (version != 0)
				ar(cereal::make_nvp("Angles", _angles), cereal::make_nvp("Speed", _speed));
			else
				ar(cereal::make_nvp("Angles", _angles));
		}

		// !Serialization
		////
		//////

#ifdef EDITOR_ENABLED
		virtual bool editorUpdate();
#endif

		glm::vec3 _angles;
		float _speed;
	};
}

CEREAL_CLASS_VERSION(AGE::RotationComponent, 1);