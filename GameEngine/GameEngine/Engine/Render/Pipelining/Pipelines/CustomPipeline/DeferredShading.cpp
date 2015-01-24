#include <Render/Pipelining/Pipelines/CustomPipeline/DeferredShading.hh>
#include <Render/UnitProg.hh>
#include <Render/Program.hh>
#include <Render/Pipelining/Render/RenderingPass.hh>
#include <Render/ProgramResources/Types/UniformBlock.hh>

#define DEFERRED_SHADING_MERGING_VERTEX "../../Shaders/Deffered_shading/deferred_shading_merge.vp"
#define DEFERRED_SHADING_MERGING_FRAG "../../Shaders/Deffered_shading/deferred_shading_merge.fp"
#define DEFERRED_SHADING_LIGHTNING_VERTEX "../../Shaders/Deffered_shading/deffered_shading_lighting.vp"
#define DEFERRED_SHADING_LIGHTNING_FRAG "../../Shaders/Deffered_shading/deffered_shading_lighting.fp"
#define DEFERRED_SHADING_BUFFERING_VERTEX "../../Shaders/Deffered_shading/deffered_shading_get_buffer.vp"
#define DEFERRED_SHADING_BUFFERING_FRAG "../../Shaders/Deffered_shading/deffered_shading_get_buffer.fp"

DeferredShading::DeferredShading(glm::mat4 const &perspective) :
ARenderingPipeline(std::string("deferred shading"))
{
	_programs.resize(TOTAL);
	_programs[BUFFERING] = std::make_shared<Program>(Program(std::string("program_buffering"), { std::make_shared<UnitProg>(DEFERRED_SHADING_BUFFERING_VERTEX, GL_VERTEX_SHADER), std::make_shared<UnitProg>(DEFERRED_SHADING_MERGING_FRAG, GL_FRAGMENT_SHADER) }));
	_programs[LIGHTNING] = std::make_shared<Program>(Program(std::string("program lightning"), { std::make_shared<UnitProg>(DEFERRED_SHADING_LIGHTNING_VERTEX, GL_VERTEX_SHADER), std::make_shared<UnitProg>(DEFERRED_SHADING_LIGHTNING_FRAG, GL_FRAGMENT_SHADER) }));
	_programs[MERGING] = std::make_shared<Program>(Program(std::string("program_merging"), { std::make_shared<UnitProg>(DEFERRED_SHADING_MERGING_VERTEX, GL_VERTEX_SHADER), std::make_shared<UnitProg>(DEFERRED_SHADING_MERGING_FRAG, GL_FRAGMENT_SHADER) }));
	*_programs[BUFFERING]->get_resource<UniformBlock>("global_state")->get_resource("projection_matrix") = perspective;
	*_programs[LIGHTNING]->get_resource<UniformBlock>("global_state")->get_resource("projection_matrix") = perspective;
	_rendering_list.resize(TOTAL);
	_rendering_list[BUFFERING] = std::make_shared<RenderingPass>([](std::vector<AGE::Drawable> const &drawables, IRenderingPipeline &_, PaintingManager &paintingManager, std::vector<std::shared_ptr<IRenderingPipeline>> const &other){
	});
	_rendering_list[LIGHTNING] = std::make_shared<RenderingPass>([](std::vector<AGE::Drawable> const &drawables, IRenderingPipeline &_, PaintingManager &paintingManager, std::vector<std::shared_ptr<IRenderingPipeline>> const &other){
	});
	_rendering_list[MERGING] = std::make_shared<RenderingPass>([](std::vector<AGE::Drawable> const &drawables, IRenderingPipeline &_, PaintingManager &paintingManager, std::vector<std::shared_ptr<IRenderingPipeline>> const &other){
	});
}

DeferredShading::DeferredShading(DeferredShading &&move) :
ARenderingPipeline(std::move(move))
{

}

IRenderingPipeline & DeferredShading::render(std::vector<AGE::Drawable> const &to_render, PaintingManager const &paintingManager)
{

}

