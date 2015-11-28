#include "DebugDrawManager.hpp"

#include <mutex>

#include <Threads/ThreadManager.hpp>
#include <Threads/MainThread.hpp>

namespace AGE
{
	void DebugDrawManager::draw3dQuad(const glm::vec3 &_a, const glm::vec3 &_b, const glm::vec3 &_c, const glm::vec3 &_d, const glm::vec3 &_color, bool _depthTest)
	{
		std::lock_guard<SpinLock> lock(_mutex);

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

	void DebugDrawManager::draw3DLine(const glm::vec3 &_start, const glm::vec3 &_startColor, const glm::vec3 &_end, const glm::vec3 &_endColor, bool _depthTest)
	{
		std::lock_guard<SpinLock> lock(_mutex);

		if (_depthTest)
		{
			_debug3DlinesPointsDepth.push_back(_start);
			_debug3DlinesPointsDepth.push_back(_end);

			_debug3DlinesColorDepth.push_back(_startColor);
			_debug3DlinesColorDepth.push_back(_endColor);
		}
		else
		{
			_debug3DlinesPoints.push_back(_start);
			_debug3DlinesPoints.push_back(_end);

			_debug3DlinesColor.push_back(_startColor);
			_debug3DlinesColor.push_back(_endColor);
		}
	}

	void DebugDrawManager::Draw2DQuad(const glm::vec2 &_a, const glm::vec2 &_b, const glm::vec2 &_c, const glm::vec2 &_d, const glm::vec3 &_color)
	{
		std::lock_guard<SpinLock> lock(_mutex);

		_debug2DlinesPoints.push_back(_a);
		_debug2DlinesPoints.push_back(_b);
		_debug2DlinesPoints.push_back(_b);
		_debug2DlinesPoints.push_back(_c);
		_debug2DlinesPoints.push_back(_c);
		_debug2DlinesPoints.push_back(_d);
		_debug2DlinesPoints.push_back(_d);
		_debug2DlinesPoints.push_back(_a);

		_debug2DlinesColor.push_back(_color);
		_debug2DlinesColor.push_back(_color);
		_debug2DlinesColor.push_back(_color);
		_debug2DlinesColor.push_back(_color);
		_debug2DlinesColor.push_back(_color);
		_debug2DlinesColor.push_back(_color);
		_debug2DlinesColor.push_back(_color);
		_debug2DlinesColor.push_back(_color);
	}

	void DebugDrawManager::draw2DLine(const glm::vec2 &_start, const glm::vec2 &_end, const glm::vec3 &_color)
	{
		std::lock_guard<SpinLock> lock(_mutex);

		_debug2DlinesPoints.push_back(_start);
		_debug2DlinesPoints.push_back(_end);

		_debug2DlinesColor.push_back(_color);
		_debug2DlinesColor.push_back(_color);
	}

	void DebugDrawManager::renderBegin(std::shared_ptr<PaintingManager> paintingManager)
	{
		// TODO : check that's called by render thread only

		SCOPE_profile_cpu_i("RenderTimer", "RenderDebugLines");
		// 2D lines
		if (_debug2DlinesPoints.size() != 0)
		{
			std::vector<unsigned int> indices;
			auto type = std::make_pair<GLenum, StringID>(GL_FLOAT_VEC2, StringID("position", 0x4cbf3a26fca1d74a));
			std::vector<std::pair < GLenum, StringID > > types;
			types.push_back(type);
			type = std::make_pair<GLenum, StringID>(GL_FLOAT_VEC3, StringID("color", 0x77f5c18e246c6638));
			types.push_back(type);

			indices.resize(_debug2DlinesPoints.size());
			for (int i = 0; i < _debug2DlinesPoints.size(); ++i)
			{
				indices[i] = i;
			}
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

			Singleton<SimpleGeometryManager>::getInstance()->debug2Dlines.verticesKey = _line2DPainter->add_vertices(_debug2DlinesPoints.size(), indices.size());
			auto vertices = _line2DPainter->get_vertices(Singleton<SimpleGeometryManager>::getInstance()->debug2Dlines.verticesKey);

			vertices->set_data<glm::vec2>(_debug2DlinesPoints, StringID("position", 0x4cbf3a26fca1d74a));
			vertices->set_data<glm::vec3>(_debug2DlinesColor, StringID("color", 0x77f5c18e246c6638));
			vertices->set_indices(indices);

			_debug2DlinesPoints.clear();
		}
		// 3D lines
		if (_debug3DlinesPoints.size() != 0)
		{
			std::vector<unsigned int> indices;
			auto type = std::make_pair<GLenum, StringID>(GL_FLOAT_VEC3, StringID("position", 0x4cbf3a26fca1d74a));
			std::vector<std::pair < GLenum, StringID > > types;
			types.push_back(type);
			type = std::make_pair<GLenum, StringID>(GL_FLOAT_VEC3, StringID("color", 0x77f5c18e246c6638));
			types.push_back(type);

			indices.resize(_debug3DlinesPoints.size());
			for (int i = 0; i < _debug3DlinesPoints.size(); ++i)
			{
				indices[i] = i;
			}
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

			Singleton<SimpleGeometryManager>::getInstance()->debug3Dlines.verticesKey = _line3DPainter->add_vertices(_debug3DlinesPoints.size(), indices.size());
			auto vertices = _line3DPainter->get_vertices(Singleton<SimpleGeometryManager>::getInstance()->debug3Dlines.verticesKey);

			vertices->set_data<glm::vec3>(_debug3DlinesPoints, StringID("position", 0x4cbf3a26fca1d74a));
			vertices->set_data<glm::vec3>(_debug3DlinesColor, StringID("color", 0x77f5c18e246c6638));
			vertices->set_indices(indices);

			_debug3DlinesPoints.clear();
		}
		// 3D lines with depth
		if (_debug3DlinesPointsDepth.size() != 0)
		{
			std::vector<unsigned int> indices;
			auto type = std::make_pair<GLenum, StringID>(GL_FLOAT_VEC3, StringID("position", 0x4cbf3a26fca1d74a));
			std::vector<std::pair < GLenum, StringID > > types;
			types.push_back(type);
			type = std::make_pair<GLenum, StringID>(GL_FLOAT_VEC3, StringID("color", 0x77f5c18e246c6638));
			types.push_back(type);

			indices.resize(_debug3DlinesPointsDepth.size());
			for (int i = 0; i < _debug3DlinesPointsDepth.size(); ++i)
			{
				indices[i] = i;
			}
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

			Singleton<SimpleGeometryManager>::getInstance()->debug3DlinesDepth.verticesKey = _line3DPainterDepth->add_vertices(_debug3DlinesPointsDepth.size(), indices.size());
			auto vertices = _line3DPainterDepth->get_vertices(Singleton<SimpleGeometryManager>::getInstance()->debug3DlinesDepth.verticesKey);

			vertices->set_data<glm::vec3>(_debug3DlinesPointsDepth, StringID("position", 0x4cbf3a26fca1d74a));
			vertices->set_data<glm::vec3>(_debug3DlinesColorDepth, StringID("color", 0x77f5c18e246c6638));
			vertices->set_indices(indices);

			_debug3DlinesPointsDepth.clear();
		}
	}

	void DebugDrawManager::renderEnd()
	{
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