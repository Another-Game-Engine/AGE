#pragma once

# include <vector>
# include <memory>
# include <Render/Program.hh>
# include <Render/Properties/AProperty.hh>
# include <glm/glm.hpp>

class Transformation : public AProperty
{
public:
	Transformation(std::vector<std::shared_ptr<Program>> const &programs);
	Transformation(glm::mat4 const &model_matrix, std::vector<std::shared_ptr<Program>> const &programs);
	Transformation(Transformation &&move);
	Transformation(Transformation const &copy) = delete;

public:
	virtual IProperty &update(std::shared_ptr<Program> const &p) override final;

public:
	glm::mat4 const &get() const;
	Transformation &set(glm::mat4 const &mat);

private:
	glm::mat4 _model_matrix;
};