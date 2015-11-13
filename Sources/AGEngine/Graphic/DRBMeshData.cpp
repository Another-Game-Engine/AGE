#include "DRBMeshData.hpp"

#include "Render\GeometryManagement\Painting\Painter.hh"
#include "Render\GeometryManagement\Data\Vertices.hh"

namespace AGE
{

	DRBMeshData::DRBMeshData()
		: DRBData()
	{
	}

	DRBMeshData::~DRBMeshData()
	{
	}

	void DRBMeshData::setPainterKey(const Key<Painter> &key)
	{
		_painter = key;
	}

	void DRBMeshData::setVerticesKey(const Key<Vertices> &key)
	{
		_vertices = key;
	}

	void DRBMeshData::setRenderMode(RenderModes mode, bool activate)
	{
		_renderMode[mode] = activate;
	}

	void DRBMeshData::setRenderModes(const RenderModeSet &modes)
	{
		_renderMode |= modes;
	}

	void DRBMeshData::setAABB(const AABoundingBox &box)
	{
		_boundingBox = box;
	}

	const Key<Painter> &DRBMeshData::getPainterKey() const
	{
		return _painter;
	}

	const Key<Vertices> &DRBMeshData::getVerticesKey() const
	{
		return _vertices;
	}

	bool DRBMeshData::hadRenderMode(RenderModes mode) const
	{
		return _renderMode.test(mode);
	}

	AABoundingBox DRBMeshData::getAABB() const
	{
		return _boundingBox;
	}
}