#include "BFCLink.hpp"
#include "BFCLinkTracker.hpp"
#include "BFCBlockManagerFactory.hpp"

#include <Utils/Debug.hpp>

namespace AGE
{
	void BFCLink::initBFC(BFCBlockManagerFactory *blockFactory, BFCLinkTracker *linkTracker)
	{
		AGE_ASSERT(blockFactory != nullptr);
		AGE_ASSERT(linkTracker != nullptr);
		_bfcBlockFactory = blockFactory;
		_bfcLinkTracker = linkTracker;
	}

	void BFCLink::registerToTracker()
	{
		if (_bfcTrackerIndex != -1)
		{
			return;
		}
		_bfcTrackerIndex = _bfcLinkTracker->addLink(this);
	}

	void BFCLink::unregisterFromTracker()
	{
		_bfcLinkTracker->removeLink(_bfcTrackerIndex);
		_bfcTrackerIndex = -1;
	}

	void BFCLink::addObject(BFCCullableObject *object)
	{
		AGE_ASSERT(object != nullptr);
		auto handle = _bfcBlockFactory->createItem(object);
		AGE_ASSERT(handle.invalid() == false);
		_cullables.push_back(handle);
	}

	void BFCLink::removeObject(const BFCCullableHandle &handle)
	{
		AGE_ASSERT(_cullables.empty() == false);

		std::size_t i = 0;
		for (auto &e : _cullables)
		{
			if (e == handle)
			{
				break;
			}
			++i;
		}
		AGE_ASSERT(handle.invalid() == false);

		std::swap(_cullables[i], _cullables.back());
		_cullables.pop_back();

		_bfcBlockFactory->deleteItem(handle);
	}
}