#pragma once

#include <glm/glm.hpp>

#include <vector>

#include "BFCCullableHandle.hpp"

namespace AGE
{
	class BFCLinkTracker;
	class BFCBlockManagerFactory;

	class BFCLink
	{
	public:
		virtual ~BFCLink() {}
		void initBFC(BFCBlockManagerFactory *blockFactory
			, BFCLinkTracker *linkTracker);
		void addObject(BFCCullableObject *object);
		void removeObject(BFCCullableObject *object);


		// dangerous, reserved to BFCLinkTracker
		void resetBFCTrackerIndex() { _bfcTrackerIndex = -1; }
	protected:
		void registerToTracker();
		void unregisterFromTracker();
		
		glm::mat4 _transformation;
		
	private:
		std::vector<BFCCullableHandle> _cullables;
		std::size_t _bfcTrackerIndex = -1;
		BFCBlockManagerFactory *_bfcBlockFactory = nullptr;
		BFCLinkTracker *_bfcLinkTracker = nullptr;
	};
}