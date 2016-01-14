#include "DebugDrawManager.hpp"

#include <mutex>

#include <Utils/OpenGL.hh>
#include <Utils/StringID.hpp>

#include <Render/GeometryManagement/Painting/PaintingManager.hh>
#include <Render/GeometryManagement/SimpleGeometryManager.hpp>

#include <Threads/ThreadManager.hpp>
#include <Threads/MainThread.hpp>

inline AGE::DebugDrawManager::SimpleVec2 convert(const glm::vec2 &v)
{
	AGE::DebugDrawManager::SimpleVec2 r{ v.x, v.y };
	return r;
}

inline AGE::DebugDrawManager::SimpleVec3 convert(const glm::vec3 &v)
{
	AGE::DebugDrawManager::SimpleVec3 r{ v.x, v.y, v.z };
	return r;
}

namespace AGE
{
	void DebugDrawManager::draw3dQuad(SimpleVec3 &&_a, SimpleVec3 &&_b, SimpleVec3 &&_c, SimpleVec3 &&_d, SimpleVec3 &&_color, bool _depthTest)
	{
		if (_depthTest)
		{
			_debug3DlinesPointsDepth.push_back(_a);
			_debug3DlinesPointsDepth.push_back(_b);
			_debug3DlinesPointsDepth.push_back(_b);
			_debug3DlinesPointsDepth.push_back(_c);
			_debug3DlinesPointsDepth.push_back(_c);
			_debug3DlinesPointsDepth.push_back(_d);
			_debug3DlinesPointsDepth.push_back(_d);
			_debug3DlinesPointsDepth.push_back(_a);

			unsigned int i = indices3DLinesDepth.size();
			for (unsigned int j = 0; j < 8; ++j)
			{
				indices3DLinesDepth.push_back(i + j);
			}

			_debug3DlinesColorDepth.push_back(_color);
			_debug3DlinesColorDepth.push_back(_color);
			_debug3DlinesColorDepth.push_back(_color);
			_debug3DlinesColorDepth.push_back(_color);
			_debug3DlinesColorDepth.push_back(_color);
			_debug3DlinesColorDepth.push_back(_color);
			_debug3DlinesColorDepth.push_back(_color);
			_debug3DlinesColorDepth.push_back(_color);
		}
		else
		{
			_debug3DlinesPoints.push_back(_a);
			_debug3DlinesPoints.push_back(_b);
			_debug3DlinesPoints.push_back(_b);
			_debug3DlinesPoints.push_back(_c);
			_debug3DlinesPoints.push_back(_c);
			_debug3DlinesPoints.push_back(_d);
			_debug3DlinesPoints.push_back(_d);
			_debug3DlinesPoints.push_back(_a);

			unsigned int i = indices3DLines.size();
			for (unsigned int j = 0; j < 8; ++j)
			{
				indices3DLines.push_back(i + j);
			}

			_debug3DlinesColor.push_back(_color);
			_debug3DlinesColor.push_back(_color);
			_debug3DlinesColor.push_back(_color);
			_debug3DlinesColor.push_back(_color);
			_debug3DlinesColor.push_back(_color);
			_debug3DlinesColor.push_back(_color);
			_debug3DlinesColor.push_back(_color);
			_debug3DlinesColor.push_back(_color);
		}
	}

	void DebugDrawManager::draw3DLine(SimpleVec3 &&_start, SimpleVec3 &&_startColor, SimpleVec3 &&_end, SimpleVec3 &&_endColor, bool _depthTest)
	{
		if (_depthTest)
		{
			_debug3DlinesPointsDepth.push_back(_start);
			_debug3DlinesPointsDepth.push_back(_end);

			unsigned int i = indices3DLinesDepth.size();
			for (unsigned int j = 0; j < 2; ++j)
			{
				indices3DLinesDepth.push_back(i + j);
			}

			_debug3DlinesColorDepth.push_back(_startColor);
			_debug3DlinesColorDepth.push_back(_endColor);
		}
		else
		{
			_debug3DlinesPoints.push_back(_start);
			_debug3DlinesPoints.push_back(_end);

			unsigned int i = indices3DLines.size();
			for (unsigned int j = 0; j < 2; ++j)
			{
				indices3DLines.push_back(i + j);
			}

			_debug3DlinesColor.push_back(_startColor);
			_debug3DlinesColor.push_back(_endColor);
		}
	}
	void DebugDrawManager::draw2DQuad(SimpleVec2 &&_a, SimpleVec2 &&_b, SimpleVec2 &&_c, SimpleVec2 &&_d, SimpleVec3 &&_color)
	{
		_debug2DlinesPoints.push_back(_a);
		_debug2DlinesPoints.push_back(_b);
		_debug2DlinesPoints.push_back(_b);
		_debug2DlinesPoints.push_back(_c);
		_debug2DlinesPoints.push_back(_c);
		_debug2DlinesPoints.push_back(_d);
		_debug2DlinesPoints.push_back(_d);
		_debug2DlinesPoints.push_back(_a);

		unsigned int i = indices2DLines.size();
		for (unsigned int j = 0; j < 8; ++j)
		{
			indices2DLines.push_back(i + j);
		}

		_debug2DlinesColor.push_back(_color);
		_debug2DlinesColor.push_back(_color);
		_debug2DlinesColor.push_back(_color);
		_debug2DlinesColor.push_back(_color);
		_debug2DlinesColor.push_back(_color);
		_debug2DlinesColor.push_back(_color);
		_debug2DlinesColor.push_back(_color);
		_debug2DlinesColor.push_back(_color);
	}

	void DebugDrawManager::draw2DLine(SimpleVec2 &&_start, SimpleVec2 &&_end, SimpleVec3 &&_color)
	{
		_debug2DlinesPoints.push_back(_start);
		_debug2DlinesPoints.push_back(_end);

		unsigned int i = indices2DLines.size();
		for (unsigned int j = 0; j < 2; ++j)
		{
			indices2DLines.push_back(i + j);
		}

		_debug2DlinesColor.push_back(_color);
		_debug2DlinesColor.push_back(_color);
	}

	void DebugDrawManager::draw3dQuad(const glm::vec3 &_a, const glm::vec3 &_b, const glm::vec3 &_c, const glm::vec3 &_d, const glm::vec3 &_color, bool _depthTest)
	{
		draw3dQuad(convert(_a), convert(_b), convert(_c), convert(_d), convert(_color), _depthTest);
	}

	void DebugDrawManager::draw3DLine(const glm::vec3 &_start, const glm::vec3 &_startColor, const glm::vec3 &_end, const glm::vec3 &_endColor, bool _depthTest)
	{
		draw3DLine(convert(_start), convert(_startColor), convert(_end), convert(_endColor), _depthTest);
	}

	void DebugDrawManager::draw2DQuad(const glm::vec2 &_a, const glm::vec2 &_b, const glm::vec2 &_c, const glm::vec2 &_d, const glm::vec3 &_color)
	{
		draw2DQuad(convert(_a), convert(_b), convert(_c), convert(_d), convert(_color));
	}

	void DebugDrawManager::draw2DLine(const glm::vec2 &_start, const glm::vec2 &_end, const glm::vec3 &_color)
	{
		draw2DLine(convert(_start), convert(_end), convert(_color));
	}

	void DebugDrawManager::renderBegin(std::shared_ptr<PaintingManager> paintingManager)
	{
		// TODO : check that's called by render thread only

		SCOPE_profile_cpu_i("RenderTimer", "RenderDebugLines");

		std::lock_guard<SpinLock> lock(_mutex);

		// 2D lines
		if (_debug2DlinesPoints.size() != 0)
		{
			auto type = std::make_pair<GLenum, StringID>(GL_FLOAT_VEC2, StringID("position", 0x4cbf3a26fca1d74a));
			std::vector<std::pair < GLenum, StringID > > types;
			types.push_back(type);
			type = std::make_pair<GLenum, StringID>(GL_FLOAT_VEC3, StringID("color", 0x77f5c18e246c6638));
			types.push_back(type);

			if (!paintingManager->has_painter(types))
			{
				Singleton<SimpleGeometryManager>::getInstance()->debug2Dlines.painterKey = paintingManager->add_painter(std::move(types));
			}
			else
			{
				Singleton<SimpleGeometryManager>::getInstance()->debug2Dlines.painterKey = paintingManager->get_painter(types);
			}
			Key<Painter> kk = Singleton<SimpleGeometryManager>::getInstance()->debug2Dlines.painterKey;

			_line2DPainter = paintingManager->get_painter(Singleton<SimpleGeometryManager>::getInstance()->debug2Dlines.painterKey);

			Singleton<SimpleGeometryManager>::getInstance()->debug2Dlines.verticesKey = _line2DPainter->add_vertices(_debug2DlinesPoints.size(), indices2DLines.size());
			auto vertices = _line2DPainter->get_vertices(Singleton<SimpleGeometryManager>::getInstance()->debug2Dlines.verticesKey);

			vertices->set_data<SimpleVec2>(_debug2DlinesPoints, StringID("position", 0x4cbf3a26fca1d74a));
			vertices->set_data<SimpleVec3>(_debug2DlinesColor, StringID("color", 0x77f5c18e246c6638));
			vertices->set_indices(indices2DLines);

		}
		// 3D lines
		if (_debug3DlinesPoints.size() != 0)
		{
			auto type = std::make_pair<GLenum, StringID>(GL_FLOAT_VEC3, StringID("position", 0x4cbf3a26fca1d74a));
			std::vector<std::pair < GLenum, StringID > > types;
			types.push_back(type);
			type = std::make_pair<GLenum, StringID>(GL_FLOAT_VEC3, StringID("color", 0x77f5c18e246c6638));
			types.push_back(type);

			if (!paintingManager->has_painter(types))
			{
				Singleton<SimpleGeometryManager>::getInstance()->debug3Dlines.painterKey = paintingManager->add_painter(std::move(types));
			}
			else
			{
				Singleton<SimpleGeometryManager>::getInstance()->debug3Dlines.painterKey = paintingManager->get_painter(types);
			}
			Key<Painter> kk = Singleton<SimpleGeometryManager>::getInstance()->debug3Dlines.painterKey;

			_line3DPainter = paintingManager->get_painter(Singleton<SimpleGeometryManager>::getInstance()->debug3Dlines.painterKey);

			Singleton<SimpleGeometryManager>::getInstance()->debug3Dlines.verticesKey = _line3DPainter->add_vertices(_debug3DlinesPoints.size(), indices3DLines.size());
			auto vertices = _line3DPainter->get_vertices(Singleton<SimpleGeometryManager>::getInstance()->debug3Dlines.verticesKey);

			vertices->set_data<SimpleVec3>(_debug3DlinesPoints, StringID("position", 0x4cbf3a26fca1d74a));
			vertices->set_data<SimpleVec3>(_debug3DlinesColor, StringID("color", 0x77f5c18e246c6638));
			vertices->set_indices(indices3DLines);

		}
		// 3D lines with depth
		if (_debug3DlinesPointsDepth.size() != 0)
		{
			auto type = std::make_pair<GLenum, StringID>(GL_FLOAT_VEC3, StringID("position", 0x4cbf3a26fca1d74a));
			std::vector<std::pair < GLenum, StringID > > types;
			types.push_back(type);
			type = std::make_pair<GLenum, StringID>(GL_FLOAT_VEC3, StringID("color", 0x77f5c18e246c6638));
			types.push_back(type);

			if (!paintingManager->has_painter(types))
			{
				Singleton<SimpleGeometryManager>::getInstance()->debug3DlinesDepth.painterKey = paintingManager->add_painter(std::move(types));
			}
			else
			{
				Singleton<SimpleGeometryManager>::getInstance()->debug3DlinesDepth.painterKey = paintingManager->get_painter(types);
			}
			Key<Painter> kk = Singleton<SimpleGeometryManager>::getInstance()->debug3DlinesDepth.painterKey;

			_line3DPainterDepth = paintingManager->get_painter(Singleton<SimpleGeometryManager>::getInstance()->debug3DlinesDepth.painterKey);

			Singleton<SimpleGeometryManager>::getInstance()->debug3DlinesDepth.verticesKey = _line3DPainterDepth->add_vertices(_debug3DlinesPointsDepth.size(), indices3DLinesDepth.size());
			auto vertices = _line3DPainterDepth->get_vertices(Singleton<SimpleGeometryManager>::getInstance()->debug3DlinesDepth.verticesKey);

			vertices->set_data<SimpleVec3>(_debug3DlinesPointsDepth, StringID("position", 0x4cbf3a26fca1d74a));
			vertices->set_data<SimpleVec3>(_debug3DlinesColorDepth, StringID("color", 0x77f5c18e246c6638));
			vertices->set_indices(indices3DLinesDepth);

		}
		_debug2DlinesPoints.clear();
		_debug2DlinesColor.clear();
		_debug3DlinesPoints.clear();
		_debug3DlinesColor.clear();
		_debug3DlinesPointsDepth.clear();
		_debug3DlinesColorDepth.clear();
		indices2DLines.clear();
		indices3DLines.clear();
		indices3DLinesDepth.clear();
	}

	void DebugDrawManager::renderEnd()
	{
		// TODO : check that's called by render thread only

		if (_line2DPainter != nullptr)
		{
			_line2DPainter->remove_vertices(Singleton<SimpleGeometryManager>::getInstance()->debug2Dlines.verticesKey);
		}
		if (_line3DPainter != nullptr)
		{
			_line3DPainter->remove_vertices(Singleton<SimpleGeometryManager>::getInstance()->debug3Dlines.verticesKey);
		}
		if (_line3DPainterDepth != nullptr)
		{
			_line3DPainterDepth->remove_vertices(Singleton<SimpleGeometryManager>::getInstance()->debug3DlinesDepth.verticesKey);
		}
	}
}
