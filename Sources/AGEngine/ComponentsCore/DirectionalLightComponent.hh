#pragma once

#include <Components/Component.hh>
#include <glm/glm.hpp>
#include <Utils/Serialization/VectorSerialization.hpp>

namespace AGE
{

	struct DirectionalLightData
	{
		DirectionalLightData(glm::vec3 const &_color = glm::vec3(1.0f));
		glm::vec3 color;
	};

	struct DirectionalLightComponent : public ComponentBase
	{
		AGE_COMPONENT_UNIQUE_IDENTIFIER("AGE_CORE_DirectionnalLightComponent");
	public:
		DirectionalLightComponent();
		virtual ~DirectionalLightComponent();

		virtual void _copyFrom(const ComponentBase *model);

		virtual void reset();
		void init();


		template <typename Archive>
		void serialize(Archive &ar, const std::uint32_t version)
		{
			ar(cereal::make_nvp("color", _data.color));
		}

		inline const DirectionalLightData &get() const { return _data; }
		DirectionalLightComponent &set(DirectionalLightData const &data);
		virtual void postUnserialization();

#ifdef EDITOR_ENABLED
		virtual bool editorUpdate();
#endif

	private:
		DirectionalLightData _data;
	};
}

CEREAL_CLASS_VERSION(AGE::DirectionalLightComponent, 0);