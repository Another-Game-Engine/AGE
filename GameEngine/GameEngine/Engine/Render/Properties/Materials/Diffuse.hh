#pragma once

# include <Render/Properties/AProperty.hh>
# include <Render/Properties/Materials/Ratio.hh>
# include <Render/Properties/Materials/Color.hh>

class Diffuse : public AProperty
{
public:
	Diffuse(std::vector<std::shared_ptr<Program>> const &programs);
	Diffuse(Diffuse &&move);

public:
	virtual IProperty &update(std::shared_ptr<Program> const &p) override final;

public:
	float get_ratio() const;
	Diffuse &set_ratio(float ratio);
	glm::vec4 const &get_color();
	Diffuse &set_color(glm::vec4 const &color);

private:
	Ratio _ratio;
	Color _color;
};