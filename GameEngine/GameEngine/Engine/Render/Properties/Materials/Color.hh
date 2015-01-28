#pragma once

# include <Render/Properties/AProperty.hh>

namespace AGE
{
	class Color : public AProperty
	{
	public:
		Color(std::string const &add_name, std::vector<std::shared_ptr<Program>> const &programs);
		Color(std::vector<std::shared_ptr<Program>> const &programs);
		Color(Color &&move);

	public:
		virtual IProperty &update(std::shared_ptr<Program> const &program) override final;

	public:
		glm::vec4 const &get() const;
		Color &set(glm::vec4 const &color);

	private:
		glm::vec4 _color;
	};
}