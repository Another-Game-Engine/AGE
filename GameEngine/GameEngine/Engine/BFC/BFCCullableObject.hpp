#pragma once

namespace AGE
{
	// all engine cullable and drawable objects
	// inherit from this type
	struct BFCCullableObject
	{
	public:
		virtual ~BFCCullableObject() {}
	};
}