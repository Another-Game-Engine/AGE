#pragma once

#include <functional>
#include <Utils/Containers/Vector.hpp>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <AssetManagement/Instance/MaterialInstance.hh>

namespace AGE
{
	class Engine;

	namespace Commands
	{
		namespace ToRender
		{
			struct Flush{
				std::size_t aligned;
			};

			struct DrawTestTriangle
			{
				std::shared_ptr<Painter> painter;
				AGE::Vector<Key<Vertices>> meshes;
				std::shared_ptr<Program> program;

				DrawTestTriangle(std::shared_ptr<Painter> const &p_painter,
					AGE::Vector<Key<Vertices>> const &p_meshes,
					std::shared_ptr<Program> const &p_proram) :
					painter(p_painter),
					meshes(p_meshes),
					program(p_proram)
				{ }
			};

			struct Draw2DLine
			{
				glm::vec2 start;
				glm::vec2 end;
				glm::vec3 color;

				Draw2DLine(glm::vec2 _start, glm::vec2 _end, glm::vec3 _color) :
					start(_start),
					end(_end),
					color(_color)
				{
				}
			};

			struct Draw2DQuad
			{
				glm::vec2 a, b, c, d;
				glm::vec3 color;

				Draw2DQuad(glm::vec2 _a, glm::vec2 _b, glm::vec2 _c, glm::vec2 _d, glm::vec3 _color) :
					a(_a),
					b(_b),
					c(_c),
					d(_d),
					color(_color)
				{
				}
			};

			struct Draw3DLine
			{
				glm::vec3 start;
				glm::vec3 end;
				glm::vec3 color;
				glm::vec3 endColor;
				bool depthTest;

				Draw3DLine(glm::vec3 _start, glm::vec3 _end, glm::vec3 _color, bool _depthTest) :
					start(_start),
					end(_end),
					color(_color),
					endColor(_color),
					depthTest(_depthTest)
				{
				}

				Draw3DLine(glm::vec3 _start, float _startColor, glm::vec3 _end, float _endColor, bool _depthTest) :
					start(_start),
					end(_end),
					color(_startColor, _startColor, _startColor),
					endColor(_endColor, _endColor, _endColor),
					depthTest(_depthTest)
				{
				}
			};

			struct Draw3DQuad
			{
				glm::vec3 a, b, c, d;
				glm::vec3 color;
				bool depthTest;

				Draw3DQuad(glm::vec3 _a, glm::vec3 _b, glm::vec3 _c, glm::vec3 _d, glm::vec3 _color, bool _depthTest) :
					a(_a),
					b(_b),
					c(_c),
					d(_d),
					color(_color),
					depthTest(_depthTest)
				{
				}
			};

		};
	}
}