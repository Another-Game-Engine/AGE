#pragma once

#include <vector>



namespace AGE
{
	struct Link;

	class BFCLinkTracker
	{
	public:
		BFCLinkTracker();
		void addLink(Link *link);
		void removeLink(Link *link);
		void reset();
	private:
		std::vector<Link*> _links;
		std::size_t        _vectorEnd;
	};
}