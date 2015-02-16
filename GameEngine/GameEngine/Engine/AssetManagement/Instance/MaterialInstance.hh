#pragma once

# include <string>
# include <vector>
# include <memory>
# include <Render/Properties/Properties.hh>

namespace AGE
{
	typedef std::vector<Key<Properties>> MaterialInstance;

	struct MaterialSetInstance
	{
	public:
		std::string name;
		std::string path;
		std::vector<MaterialInstance> datas;
	};
}