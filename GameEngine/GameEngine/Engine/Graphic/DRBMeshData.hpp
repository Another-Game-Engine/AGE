#pragma once

#include "DRBData.hpp"
#include "Render\Pipelining\Render\RenderModes.hh"

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
		void setRenderMode(RenderModes mode, bool activate);
		void setRenderModes(const RenderModeSet &modes);
		const Key<Painter> getPainterKey() const;
		const Key<Vertices> getVerticesKey() const;
		bool hadRenderMode(RenderModes mode) const;

	private:
		Key<Painter> _painter;
		Key<Vertices> _vertices;
		RenderModeSet _renderMode;
	};
}