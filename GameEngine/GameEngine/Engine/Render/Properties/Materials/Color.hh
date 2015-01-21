#pragma once

# include <Render/Properties/AProperty.hh>

class Color : public AProperty
{
public:
	Color(std::string const &add_name);
	Color(Color &&move);

public:
	virtual IProperty &update(std::shared_ptr<Program> const &program) override final;

public:
	glm::vec4 const &get() const;
	Color &set(glm::vec4 const &color);

private:
	glm::vec4 _color;
};