#pragma once

# include <vector>
# include <memory>
# include <Render/Program.hh>
# include <Render/Properties/AProperty.hh>
# include <glm/glm.hpp>

namespace AGE
{
	class Transformation : public AProperty
	{
	public:
		Transformation(glm::mat4 const &model_matrix);
		Transformation(Transformation &&move);
		Transformation(Transformation const &copy) = delete;
		Transformation &operator=(Transformation const &other) = delete;
		Transformation &operator=(glm::mat4 const &value);

	private:
		virtual void _update(IProgramResources *p) override final;
		virtual void _update_instancied(IProgramResources *p) override final;

	public:
		glm::mat4 const &get();
		Transformation &set(glm::mat4 const &mat);

	private:
		glm::mat4 _model_matrix;
	};
}
