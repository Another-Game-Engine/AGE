#pragma once

#include <vector>
#include <glm/glm.hpp>

#include <Utils/Spinlock.hpp>
#include <Utils/Dependency.hpp>
#include <Utils/Containers/PODVector.hpp>

namespace AGE
{
	class PaintingManager;
	class Painter;

	class DebugDrawManager : public Dependency < DebugDrawManager >
	{
	public:
		struct SimpleVec2
		{
			float x, y;
		};
		struct SimpleVec3
		{
			float x, y, z;
		};
		inline void lock() { _mutex.lock(); }
		inline void unlock() { _mutex.unlock(); }
		// IMPORTANT LOCK IT AND UNLOCKIT
		void draw3dQuad(SimpleVec3 &&_a, SimpleVec3 &&_b, SimpleVec3 &&_c, SimpleVec3 &&_d, SimpleVec3 &&_color, bool _depthTest);
		// IMPORTANT LOCK IT AND UNLOCKIT
		void draw3DLine(SimpleVec3 &&_start, SimpleVec3 &&_startColor, SimpleVec3 &&_end, SimpleVec3 &&_endColor, bool _depthTest);
		// IMPORTANT LOCK IT AND UNLOCKIT
		void draw2DQuad(SimpleVec2 &&_a, SimpleVec2 &&_b, SimpleVec2 &&_c, SimpleVec2 &&_d, SimpleVec3 &&_color);
		// IMPORTANT LOCK IT AND UNLOCKIT
		void draw2DLine(SimpleVec2 &&_start, SimpleVec2 &&_end, SimpleVec3 &&_color);
		// IMPORTANT LOCK IT AND UNLOCKIT
		void draw3dQuad(const glm::vec3 &_a, const glm::vec3 &_b, const glm::vec3 &_c, const glm::vec3 &_d, const glm::vec3 &_color, bool _depthTest);
		// IMPORTANT LOCK IT AND UNLOCKIT
		void draw3DLine(const glm::vec3 &_start, const glm::vec3 &_startColor, const glm::vec3 &_end, const glm::vec3 &_endColor, bool _depthTest);
		// IMPORTANT LOCK IT AND UNLOCKIT
		void draw2DQuad(const glm::vec2 &_a, const glm::vec2 &_b, const glm::vec2 &_c, const glm::vec2 &_d, const glm::vec3 &_color);
		// IMPORTANT LOCK IT AND UNLOCKIT
		void draw2DLine(const glm::vec2 &_start, const glm::vec2 &_end, const glm::vec3 &_color);
		void renderBegin(std::shared_ptr<PaintingManager> paintingManager);
		void renderEnd();
	private:
		// Debug Drawings
		// 2D lines
		PODVector<SimpleVec2> _debug2DlinesPoints;
		PODVector<SimpleVec3> _debug2DlinesColor;

		PODVector<SimpleVec3> _debug3DlinesPoints;
		PODVector<SimpleVec3> _debug3DlinesColor;
		// 3D lines with depth test
		PODVector<SimpleVec3> _debug3DlinesPointsDepth;
		PODVector<SimpleVec3> _debug3DlinesColorDepth;

		SpinLock _mutex;

		std::shared_ptr<Painter> _line2DPainter = nullptr;
		std::shared_ptr<Painter> _line3DPainter = nullptr;
		std::shared_ptr<Painter> _line3DPainterDepth = nullptr;
	};
}