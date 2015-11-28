#pragma once

#include <vector>
#include <glm/glm.hpp>

#include <Utils/Spinlock.hpp>
#include <Utils/Dependency.hpp>

namespace AGE
{
	class DebugDrawManager : public Dependency < DebugDrawManager >
	{
	public:
		void draw3dQuad(const glm::vec3 &_a, const glm::vec3 &_b, const glm::vec3 &_c, const glm::vec3 &_d, const glm::vec3 &_color, bool _depthTest);
		void draw3DLine(const glm::vec3 &_start, const glm::vec3 &_startColor, const glm::vec3 &_end, const glm::vec3 &_endColor, bool _depthTest);
		void draw2DQuad(const glm::vec2 &_a, const glm::vec2 &_b, const glm::vec2 &_c, const glm::vec2 &_d, const glm::vec3 &_color);
		void draw2DLine(const glm::vec2 &_start, const glm::vec2 &_end, const glm::vec3 &_color);
		void renderBegin(std::shared_ptr<PaintingManager> paintingManager);
		void renderEnd();
	private:
		// Debug Drawings
		// 2D lines
		std::vector<glm::vec2> _debug2DlinesPoints;
		std::vector<glm::vec3> _debug2DlinesColor;

		std::vector<glm::vec3> _debug3DlinesPoints;
		std::vector<glm::vec3> _debug3DlinesColor;
		// 3D lines with depth test
		std::vector<glm::vec3> _debug3DlinesPointsDepth;
		std::vector<glm::vec3> _debug3DlinesColorDepth;

		SpinLock _mutex;

		std::shared_ptr<Painter> _line2DPainter = nullptr;
		std::shared_ptr<Painter> _line3DPainter = nullptr;
		std::shared_ptr<Painter> _line3DPainterDepth = nullptr;
	};
}