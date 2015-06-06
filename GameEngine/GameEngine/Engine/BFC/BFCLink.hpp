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
		virtual ~BFCLink();
		void initBFC(BFCBlockManagerFactory *blockFactory
			, BFCLinkTracker *linkTracker);
		void addObject(BFCCullableObject *object);
		// dangerous !
		// Be sure to not push duplicates
		// It's safer to use the other method
		void pushAnObject(const BFCCullableHandle &handle);
		void removeObject(const BFCCullableHandle &handle);
		// dangerous !
		// Be sure to not pop items without removing them from blockfactory
		// It's safer to use the other method
		void popAnObject(const BFCCullableHandle &handle);
		inline void setTransform(const glm::mat4 &transformation) { _globalTransformation = transformation; }
		inline glm::mat4 getTransform() const { return _globalTransformation; }
	protected:
		void registerToTracker();
		void unregisterFromTracker();

		// called by LinkTracker
		void resetBFCTrackerIndex() { _bfcTrackerIndex = -1; }

		glm::mat4 _globalTransformation;
	private:
		std::vector<BFCCullableHandle> _cullables;
		std::size_t _bfcTrackerIndex = -1;
		BFCBlockManagerFactory *_bfcBlockFactory = nullptr;
		BFCLinkTracker *_bfcLinkTracker = nullptr;
		
		friend class BFCLinkTracker;
	};
}