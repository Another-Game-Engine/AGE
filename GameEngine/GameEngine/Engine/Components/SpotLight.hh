#pragma once

#include <Components/Component.hh>
#include <glm/fwd.hpp>
#include <Utils/Serialization/VectorSerialization.hpp>
#include <Render/Textures/ITexture.hh>

namespace AGE
{

	struct SpotLightComponent : public ComponentBase
	{
	public:
		SpotLightComponent();
		virtual ~SpotLightComponent() = default;
		SpotLightComponent(SpotLightComponent const &o);

		virtual void _copyFrom(const ComponentBase *model);

		virtual void reset();
		void init();

		SpotLightComponent &set(glm::vec3 const &color, glm::vec3 const &range, float cutOff, float exponent);

		template <typename Archive>
		void serialize(Archive &ar, const std::uint32_t version)
		{
			ar(cereal::make_nvp("color", _color), cereal::make_nvp("exponent", _exponent), cereal::make_nvp("cutOff", _cutOff));
		}

		inline const glm::vec3 &getColor() const { return _color; }
		inline const glm::vec3 &getRange() const { return _range;  }
		inline float getCutOff() const { return _cutOff;  }
		inline const float getExponent() const { return _exponent; }
		inline float *getColorPtr() { return &_color.x; }
		inline float *getRangePtr() { return &_range.x; }
		virtual void postUnserialization();

#ifdef EDITOR_ENABLED
		virtual void editorCreate();
		virtual void editorDelete();
		virtual bool editorUpdate();
#endif

	private:
		AGE::PrepareKey _key;
		glm::vec3 _color;
		glm::vec3 _range;
		float _exponent;
		float _cutOff;
		std::shared_ptr<ITexture> _map;

	};
}