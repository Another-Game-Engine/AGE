#pragma once

# include <Render/Properties/AProperty.hh>
# include <Render/Properties/Materials/Ratio.hh>
# include <Render/Properties/Materials/Color.hh>
# include <Render/Properties/Materials/MapColor.hh>

namespace AGE
{
	class Texture2D;

	class Diffuse : public AProperty
	{
	public:
		Diffuse();
		Diffuse(Diffuse &&move);

	public:
		virtual IProperty &update(std::shared_ptr<Program> const &p) override final;
		virtual IProperty &set_program(std::vector<std::shared_ptr<Program>> const &programs) override final;

	public:
		float get_ratio() const;
		Diffuse &set_ratio(float ratio);
		glm::vec4 const &get_color();
		Diffuse &set_color(glm::vec4 const &color);
		std::shared_ptr<Texture2D> const &get_map() const;
		Diffuse &set_map(std::shared_ptr<Texture2D> const &m);

	private:
		Ratio _ratio;
		Color _color;
		MapColor _mapColor;
	};
}
