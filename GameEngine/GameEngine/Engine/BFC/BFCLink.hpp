#pragma once

#include <glm/glm.hpp>

namespace AGE
{
	class BFCLink
	{
	public:
		virtual ~BFCLink() {}
	private:
		glm::mat4 _transformation;
		std::vector<BFCCulledHandle>
	};
}