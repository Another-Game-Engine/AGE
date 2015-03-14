#include <Render/Pipelining/Pipelines/CustomPipeline/BasicPipeline.hh>
#include <Render/ProgramResources/Types/Uniform/Mat4.hh>
#include <Render/ProgramResources/Types/Uniform/Mat4Array255.hh>
#include <Render/ProgramResources/Types/Uniform/Vec1.hh>
#include <Render/Program.hh>
#include <Render/Pipelining/Render/Rendering.hh>
#include <Render/OpenGLTask/Tasks.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Render/Properties/Properties.hh>
#include <Render/GeometryManagement/Painting/PaintingManager.hh>
#include <SpacePartitioning/Ouptut/RenderPipeline.hh>
#include <SpacePartitioning/Ouptut/RenderPainter.hh>
#include <SpacePartitioning/Ouptut/RenderCamera.hh>
#include <vector>
#include <memory>

#define VERTEX_SHADER_BASIC "../../Shaders/test_pipeline_1.vp", GL_VERTEX_SHADER
#define FRAGMENT_SHADER_BASIC "../../Shaders/test_pipeline_1.fp", GL_FRAGMENT_SHADER

#define VERTEX_SHADER_SKIN "../../Shaders/test_pipeline_1_skinning.vp", GL_VERTEX_SHADER
#define FRAGMENT_SHADER_SKIN "../../Shaders/test_pipeline_1.fp", GL_FRAGMENT_SHADER

//DIRTY
#include <glm/gtc/type_ptr.hpp>

std::vector<glm::mat4> DirtyBoneContainer::BONES = std::vector<glm::mat4>();
std::mutex DirtyBoneContainer::BONES_MUTEX = std::mutex();

namespace AGE
{

	BasicPipeline::BasicPipeline(std::shared_ptr<PaintingManager> const &painter_manager) :
		ARenderingPipeline(std::string("BasicName"), painter_manager)
	{
		_programs.resize(TOTAL);
		std::vector<std::shared_ptr<UnitProg>> unitsBasic = { std::make_shared<UnitProg>(VERTEX_SHADER_BASIC), std::make_shared<UnitProg>(FRAGMENT_SHADER_BASIC) };
		std::vector<std::shared_ptr<UnitProg>> unitsSkin = { std::make_shared<UnitProg>(VERTEX_SHADER_SKIN), std::make_shared<UnitProg>(FRAGMENT_SHADER_SKIN) };
		_programs[RENDER_SKINNED] = std::make_shared<Program>(Program(std::string("basic program skin"), unitsSkin));
		_programs[RENDER_BASIC] = std::make_shared<Program>(Program(std::string("basic program"), unitsBasic));
		_rendering_list.resize(TOTAL);
		_rendering_list[RENDER_SKINNED] = std::make_shared<Rendering>([&](FUNCTION_ARGS) {
			painter->draw(GL_TRIANGLES, _programs[RENDER_SKINNED], properties, vertices);
		});
		_rendering_list[RENDER_BASIC] = std::make_shared<Rendering>([&](FUNCTION_ARGS) {
			painter->draw(GL_TRIANGLES, _programs[RENDER_BASIC], properties, vertices);
		});
//		auto &rendering = std::static_pointer_cast<Rendering>(_rendering_list[RENDER]);
	}

	BasicPipeline::BasicPipeline(BasicPipeline &&move) :
		ARenderingPipeline(std::move(move))
	{
	}

	IRenderingPipeline &BasicPipeline::render(RenderPipeline const &pipeline, RenderLightList const &lights, CameraInfos const &infos)
	{
		OpenGLTasks::set_depth_test(true);
		OpenGLTasks::set_clear_color(glm::vec4(0, 0, 0.2, 1));
		OpenGLTasks::clear_buffer();

		//*_programs[RENDER]->get_resource<Mat4Array255>("bones") = &(infos.view);

		for (auto key : pipeline.keys) {
			auto &curPainter = _painter_manager->get_painter(key.painter);
			int currentRenderIdx = -1;
			if (curPainter->coherent(_programs[RENDER_SKINNED]))
			{ 
				currentRenderIdx = RENDER_SKINNED;
			}
			else if (curPainter->coherent(_programs[RENDER_BASIC]))
			{
				currentRenderIdx = RENDER_BASIC;
			}
			assert(currentRenderIdx != -1);

			if (!_programs[currentRenderIdx]->isCompiled())
			{
				continue;
			}

			_programs[currentRenderIdx]->use();
			if (currentRenderIdx == RENDER_SKINNED)
			{
				if (!_programs[RENDER_SKINNED]->isCompiled())
				{
					continue;
				}
				*_programs[RENDER_SKINNED]->get_resource<Vec1>("skinned") = 1.0f;
				auto bones = DirtyBoneContainer::getBones();
				if (bones.size() > 0)
				{
					auto bonesId = _programs[RENDER_SKINNED]->get_resource<Mat4>("bones[0]")->id();
					glUniformMatrix4fv(bonesId, (GLsizei)bones.size(), GL_FALSE, (GLfloat *)(glm::value_ptr(bones[0])));
				}
			}

			auto test = glGetUniformLocation(_programs[currentRenderIdx]->id(), "projection_matrix");
				
			*_programs[currentRenderIdx]->get_resource<Mat4>("projection_matrix") = infos.projection;
			*_programs[currentRenderIdx]->get_resource<Mat4>("view_matrix") = infos.view;

			_rendering_list[currentRenderIdx]->render(key.properties, key.vertices, curPainter);
		}
		return (*this);
	}

}