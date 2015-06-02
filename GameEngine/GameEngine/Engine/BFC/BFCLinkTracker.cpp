#include "BFCLinkTracker.hpp"
#include <Core/Link.hpp>
#include <Utils/Debug.hpp>
#include <Utils/Profiler.hpp>

namespace AGE
{
	BFCLinkTracker::BFCLinkTracker()
		: _vectorEnd(0)
	{}

	void BFCLinkTracker::addLink(Link *link)
	{
		SCOPE_profile_cpu_function("BFC");
		if (link->getBFCTrackerIndex() == -1)
		{
			return;
		}
		link->setBFCTrackerIndex(_vectorEnd);
		if (_links.size() <= _vectorEnd)
		{
			_links.resize(_links.size() + 16);
		}
		_links[_vectorEnd] = link;
		++_vectorEnd;
	}

	void BFCLinkTracker::removeLink(Link *link)
	{
		SCOPE_profile_cpu_function("BFC");
		auto index = link->getBFCTrackerIndex();
		AGE_ASSERT(index != -1 && index < _vectorEnd);
		if (index == --_vectorEnd)
		{
			return;
		}
		std::swap(_links[index], _links[_vectorEnd + 1]);
		link->setBFCTrackerIndex(-1);
	}

	void BFCLinkTracker::reset()
	{
		SCOPE_profile_cpu_function("BFC");
		for (auto i = 0; i < _vectorEnd; ++i)
		{
			_links[i]->setBFCTrackerIndex(-1);
		}
		_vectorEnd = 0;
	}
}