#pragma once


#include <Components/Component.hh>
#include <glm/fwd.hpp>
#include <Utils/Serialization/VectorSerialization.hpp>

namespace AGE
{

	struct DirectionalLightData
	{
		DirectionalLightData(glm::vec3 const &color = glm::vec3(1.0f));
		glm::vec3 color;
	};

	struct DirectionalLightComponent : public ComponentBase
	{
	public:
		DirectionalLightComponent();
		virtual ~DirectionalLightComponent() = default;
		DirectionalLightComponent(DirectionalLightComponent const &o) = default;

		virtual void _copyFrom(const ComponentBase *model);

		virtual void reset();
		void init();


		template <typename Archive>
		void serialize(Archive &ar, const std::uint32_t version)
		{
			ar(cereal::make_nvp("data", _data));
		}

		inline const DirectionalLightData &get() const { return _data; }
		DirectionalLightComponent &set(DirectionalLightData const &data);
		virtual void postUnserialization();

#ifdef EDITOR_ENABLED
		virtual void editorCreate();
		virtual void editorDelete();
		virtual bool editorUpdate();
#endif

	private:
		AGE::PrepareKey _key;
		DirectionalLightData _data;
	};
}