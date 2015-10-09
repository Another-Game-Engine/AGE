#include "BFCLink.hpp"
#include "BFCLinkTracker.hpp"
#include "BFCBlockManagerFactory.hpp"
#include <Utils/Profiler.hpp>
#include <Utils/Debug.hpp>

namespace AGE
{
	BFCLink::~BFCLink()
	{
		unregisterFromTracker();
	}

	BFCLink::BFCLink()
	{
	}

	void BFCLink::initBFC(BFCBlockManagerFactory *blockFactory, BFCLinkTracker *linkTracker)
	{
		AGE_ASSERT(blockFactory != nullptr);
		AGE_ASSERT(linkTracker != nullptr);
		_bfcBlockFactory = blockFactory;
		_bfcLinkTracker = linkTracker;
	}

	void BFCLink::registerToTracker()
	{
		SCOPE_profile_cpu_function("BFC");

		if (_bfcTrackerIndex != -1)
		{
			return;
		}
		_bfcTrackerIndex = _bfcLinkTracker->addLink(this);
	}

	void BFCLink::unregisterFromTracker()
	{
		if (_bfcTrackerIndex == -1)
		{
			return;
		}
		_bfcLinkTracker->removeLink(_bfcTrackerIndex);
		_bfcTrackerIndex = -1;
	}

	void BFCLink::addObject(BFCCullableObject *object)
	{
		AGE_ASSERT(object != nullptr);
		auto handle = _bfcBlockFactory->createItem(object);
		AGE_ASSERT(handle.invalid() == false);
		_cullables.push_back(handle);
		registerToTracker();
	}

	// dangerous !
	// Be sure to not push duplicates
	// It's safer to use the other method
	void BFCLink::pushAnObject(const BFCCullableHandle &handle)
	{
		_cullables.push_back(handle);
		registerToTracker();
	}

	void BFCLink::removeObject(const BFCCullableHandle &handle)
	{
		popAnObject(handle);
		_bfcBlockFactory->deleteItem(handle);
		unregisterFromTracker();
	}

	// dangerous !
	// Be sure to not pop items without removing them from blockfactory
	// It's safer to use the other method
	void BFCLink::popAnObject(const BFCCullableHandle &handle)
	{
		AGE_ASSERT(_cullables.empty() == false);

		std::size_t i = 0;
		bool found = false;
		for (auto &e : _cullables)
		{
			if (e == handle)
			{
				found = true;
				break;
			}
			++i;
		}
		AGE_ASSERT(handle.invalid() == false);
		AGE_ASSERT(found);

		std::swap(_cullables[i], _cullables.back());
		_cullables.pop_back();
		unregisterFromTracker();
	}

	void BFCLink::resetBFCTrackerIndex()
	{
		_bfcTrackerIndex = -1;
	}

}