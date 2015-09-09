#pragma once

#include "DRBData.hpp"
#include "Render\Pipelining\Render\RenderModes.hh"
#include "Utils/AABoundingBox.hh"

namespace AGE
{
	class Painter;
	class Vertices;

	struct DRBMeshData : public DRBData
	{
		DRBMeshData();
		virtual ~DRBMeshData();

		void setPainterKey(const Key<Painter> &key);
		void setVerticesKey(const Key<Vertices> &key);
		void setRenderMode(RenderModes mode, bool activate);
		void setRenderModes(const RenderModeSet &modes);
		void setAABB(const AABoundingBox &box);
		const Key<Painter> getPainterKey() const;
		const Key<Vertices> getVerticesKey() const;
		bool hadRenderMode(RenderModes mode) const;
		AABoundingBox getAABB() const;

	private:
		Key<Painter> _painter;
		Key<Vertices> _vertices;
		RenderModeSet _renderMode;
		AABoundingBox _boundingBox;
	};
}