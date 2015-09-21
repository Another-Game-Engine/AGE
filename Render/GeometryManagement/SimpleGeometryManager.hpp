#pragma once

#include <Utils/Singleton.hh>
#include "SimpleGeometry.hh"

namespace AGE
{
	class SimpleGeometryManager
	{
		friend class Singleton < SimpleGeometryManager >;
	public:
		SimpleGeometry::SimpleGeometryKeys debug2Dlines;
		SimpleGeometry::SimpleGeometryKeys debug3Dlines;
		SimpleGeometry::SimpleGeometryKeys debug3DlinesDepth;
	};
}