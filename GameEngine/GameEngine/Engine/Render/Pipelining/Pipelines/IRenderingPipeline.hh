#pragma once

# include <vector>
# include <string>
# include <memory>
# include <Render/GeometryManagement/Painting/PaintingManager.hh>
# include <Core/CullableObjects.hh>

class IRendering;
class Painter;
class Program;

class IRenderingPipeline
{
public:
	virtual ~IRenderingPipeline() {}
	virtual IRenderingPipeline &set_rendering(std::vector<std::shared_ptr<IRendering>> const &rendering_list) = 0;
	virtual std::vector<std::shared_ptr<IRendering>> const &get_rendering() const = 0;
	virtual std::string const &name() const = 0;
	virtual IRenderingPipeline &render(std::vector<AGE::Drawable> const &to_render, PaintingManager const &paintingManager) = 0;
	virtual std::vector<std::shared_ptr<Program>> const &get_programs() const = 0;
};