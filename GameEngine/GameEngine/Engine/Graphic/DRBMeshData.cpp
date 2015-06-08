#include "DRBMeshData.hpp"

#include "Render\GeometryManagement\Painting\Painter.hh"
#include "Render\GeometryManagement\Data\Vertices.hh"

namespace AGE
{
	void DRBMeshData::setPainterKey(const Key<Painter> &key)
	{
		RWLockGuard(_lock, true);
		_painter = key;
	}

	void DRBMeshData::setVerticesKey(const Key<Vertices> &key)
	{
		RWLockGuard(_lock, true);
		_vertices = key;
	}

	const Key<Painter> DRBMeshData::getPainterKey() const
	{
		RWLockGuard(_lock, false);
		return _painter;
	}

	const Key<Vertices> DRBMeshData::getVerticesKey() const
	{
		RWLockGuard(_lock, false);
		return _vertices;
	}
}