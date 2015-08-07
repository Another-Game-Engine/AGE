#pragma once

#include "DRBData.hpp"

namespace AGE
{
	class Painter;
	class Vertices;

	struct DRBMeshData : public DRBData
	{
		DRBMeshData()
			: DRBData()
		{
		}

		virtual ~DRBMeshData()
		{
		}

		void setPainterKey(const Key<Painter> &key);
		void setVerticesKey(const Key<Vertices> &key);
		const Key<Painter> getPainterKey() const;
		const Key<Vertices> getVerticesKey() const;
	private:
		Key<Painter> _painter;
		Key<Vertices> _vertices;
	};
}