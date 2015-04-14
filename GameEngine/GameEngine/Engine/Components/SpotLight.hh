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
		virtual ~SpotLightComponent();
		SpotLightComponent(SpotLightComponent const &o);

		virtual void _copyFrom(const ComponentBase *model);

		virtual void reset();
		void init();

		SpotLightComponent &set(glm::vec3 const &color = glm::vec3(1.0f), glm::vec3 const &range = glm::vec3(1.0f, 0.1f, 0.01f));

		template <typename Archive>void serialize(Archive &ar, const std::uint32_t version);
		inline const glm::vec3 &getColor() const { return _color; }
		inline const glm::vec3 &getRange() const { return _range; }
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
		glm::mat4 _view;
		glm::mat4 _projection;
		glm::vec3 _color;
		glm::vec3 _range;
		std::shared_ptr<ITexture> _map;

	};
}