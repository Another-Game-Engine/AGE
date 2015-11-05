#include <Render/Pipelining/Pipelines/CustomRenderPass/DebugDrawLines.hh>

#include <Render/Textures/Texture2D.hh>
#include <Render/OpenGLTask/OpenGLState.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Render/GeometryManagement/SimpleGeometryManager.hpp>
#include <Render/GeometryManagement/SimpleGeometry.hh>
#include <Render/ProgramResources/Types/Uniform/Mat4.hh>
#include <Render/ProgramResources/Types/Uniform/Sampler/Sampler2D.hh>
#include <Render/ProgramResources/Types/Uniform/Vec3.hh>

#include <Threads/RenderThread.hpp>
#include <Threads/ThreadManager.hpp>

#include <Core/ConfigurationManager.hpp>
#include <Core/Engine.hh>

#include <Graphic/DRBCameraDrawableList.hpp>

#define DRAW_2D_LINE_VERTEX "deferred_shading/draw2DLine.vp"
#define DRAW_2D_LINE_FRAG "deferred_shading/draw2DLine.fp"

#define DRAW_3D_LINE_VERTEX "deferred_shading/draw3DLine.vp"
#define DRAW_3D_LINE_FRAG "deferred_shading/draw3DLine.fp"

namespace AGE
{
	enum Programs
	{
		PROGRAM_DRAW_2D_LINE,
		PROGRAM_DRAW_3D_LINE,
		PROGRAM_NBR
	};

	DebugDrawLines::DebugDrawLines(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
		std::shared_ptr<Texture2D> diffuse, std::shared_ptr<Texture2D> depth) :
		FrameBufferRender(screenSize.x, screenSize.y, painterManager)
	{
		_diffuseInput = diffuse;
		_programs.resize(PROGRAM_NBR);

		push_storage_output(GL_COLOR_ATTACHMENT0, diffuse);
		push_storage_output(GL_DEPTH_STENCIL_ATTACHMENT, depth);

		auto confManager = GetEngine()->getInstance<ConfigurationManager>();

		auto shaderPath = confManager->getConfiguration<std::string>("ShadersPath");

		auto vertexDrawLine = shaderPath->getValue() + DRAW_2D_LINE_VERTEX;
		auto fragDrawLine = shaderPath->getValue() + DRAW_2D_LINE_FRAG;

		_programs[PROGRAM_DRAW_2D_LINE] = std::make_shared<Program>(Program(std::string("draw2DLine"),
		{
			std::make_shared<UnitProg>(vertexDrawLine, GL_VERTEX_SHADER),
			std::make_shared<UnitProg>(fragDrawLine, GL_FRAGMENT_SHADER)
		}));

		vertexDrawLine = shaderPath->getValue() + DRAW_3D_LINE_VERTEX;
		fragDrawLine = shaderPath->getValue() + DRAW_3D_LINE_FRAG;

		_programs[PROGRAM_DRAW_3D_LINE] = std::make_shared<Program>(Program(std::string("draw3DLine"),
		{
			std::make_shared<UnitProg>(vertexDrawLine, GL_VERTEX_SHADER),
			std::make_shared<UnitProg>(fragDrawLine, GL_FRAGMENT_SHADER)
		}));

		Singleton<SimpleGeometryManager>::setInstance();

		Key<Painter> quadPainterKey;
	}

	void DebugDrawLines::renderPass(const DRBCameraDrawableList &infos)
	{
		OpenGLState::glDisable(GL_DEPTH_TEST);
		OpenGLState::glDisable(GL_BLEND);
		OpenGLState::glDisable(GL_CULL_FACE);
		OpenGLState::glDisable(GL_STENCIL_TEST);
		OpenGLState::glDepthMask(GL_FALSE);
		OpenGLState::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		_programs[PROGRAM_DRAW_2D_LINE]->use();

		std::shared_ptr<AGE::Painter> lines;
		auto &_2dLines = Singleton<SimpleGeometryManager>::getInstance()->debug2Dlines;

		if (_2dLines.verticesKey.isValid() && _2dLines.painterKey.isValid())
		{
			lines = _painterManager->get_painter(_2dLines.painterKey);
			lines->uniqueDrawBegin(_programs[PROGRAM_DRAW_2D_LINE]);
			lines->uniqueDraw(GL_LINES, _programs[PROGRAM_DRAW_2D_LINE], _2dLines.verticesKey);
			lines->uniqueDrawEnd();
		}

		_programs[PROGRAM_DRAW_3D_LINE]->use();
		_programs[PROGRAM_DRAW_3D_LINE]->get_resource<Mat4>("viewProj").set(infos.cameraInfos.data.projection * infos.cameraInfos.view);

		auto _3dLines = Singleton<SimpleGeometryManager>::getInstance()->debug3Dlines;
		if (_3dLines.verticesKey.isValid() && _3dLines.painterKey.isValid())
		{
			lines = _painterManager->get_painter(_3dLines.painterKey);
			lines->uniqueDrawBegin(_programs[PROGRAM_DRAW_3D_LINE]);
			lines->uniqueDraw(GL_LINES, _programs[PROGRAM_DRAW_3D_LINE], _3dLines.verticesKey);
			lines->uniqueDrawEnd();
		}

		OpenGLState::glEnable(GL_DEPTH_TEST);
		OpenGLState::glDepthFunc(GL_LESS);

		auto &_3dlinesDepth = Singleton<SimpleGeometryManager>::getInstance()->debug3DlinesDepth;
		if (_3dlinesDepth.painterKey.isValid() && _3dlinesDepth.painterKey.isValid())
		{
			lines = _painterManager->get_painter(_3dlinesDepth.painterKey);
			lines->uniqueDrawBegin(_programs[PROGRAM_DRAW_3D_LINE]);
			lines->uniqueDraw(GL_LINES, _programs[PROGRAM_DRAW_3D_LINE], _3dlinesDepth.verticesKey);
			lines->uniqueDrawEnd();
		}
	}
}
