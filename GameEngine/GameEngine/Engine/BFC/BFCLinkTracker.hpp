#pragma once

#include <vector>
#include <queue>

namespace AGE
{
	class BFCLink;

	class BFCLinkTracker
	{
	public:
		BFCLinkTracker();
		std::size_t addLink(BFCLink *link);
		void removeLink(std::size_t link);
		void reset();
		inline std::size_t getLinkCount() const { return _links.size(); }
	private:
		std::vector<BFCLink*> _links;
		std::queue<std::size_t> _free;
	};
}