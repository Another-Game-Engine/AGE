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

	public:
		virtual IProperty &update(std::shared_ptr<Program> const &p) override final;

	public:
		glm::mat4 const &get() const;
		Transformation &set(glm::mat4 const &mat);

	private:
		glm::mat4 _model_matrix;
	};
}
