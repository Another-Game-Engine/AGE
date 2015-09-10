#include "BFCLinkTracker.hpp"
#include "BFCBlockManagerFactory.hpp"

#include "BFCCullableObject.hpp"
#include "BFCItem.hpp"

#include <Core/Link.hpp>
#include <Utils/Debug.hpp>
#include <Utils/Profiler.hpp>

namespace AGE
{
	BFCLinkTracker::BFCLinkTracker()
	{}

	std::size_t BFCLinkTracker::addLink(BFCLink *link)
	{
		SCOPE_profile_cpu_function("BFC");

		std::size_t result = -1;

		if (_free.empty() == false)
		{
			result = _free.front();
			_links[result] = link;
			_free.pop();
		}
		else
		{
			result = _links.size();
			_links.push_back(link);
		}
		return result;
	}

	void BFCLinkTracker::removeLink(std::size_t index)
	{
		SCOPE_profile_cpu_function("BFC");

		AGE_ASSERT(index != -1 && index < _links.size());
		_links[index] = nullptr;
		_free.push(index);
	}

	void BFCLinkTracker::reset()
	{
		SCOPE_profile_cpu_function("BFC");
		{
			SCOPE_profile_cpu_i("BFC", "ResetLinks");
			for (auto &e : _links)
			{
				// beurk
				if (e != nullptr)
				{
					e->resetBFCTrackerIndex();
					for (auto &cullable : e->_cullables)
					{
						auto &item = e->_bfcBlockFactory->getItem(cullable.getItemId());
						item.setPosition(cullable.getPtr()->setBFCTransform(e->_globalTransformation));
					}
					e = nullptr;
				}
			}
			_links.clear();
		}
		{
			SCOPE_profile_cpu_i("BFC", "ResetFree");
			//beurk
			while (!_free.empty())
				_free.pop();
		}
	}

}