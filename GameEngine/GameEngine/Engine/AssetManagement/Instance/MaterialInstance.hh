#pragma once

# include <string>
# include <vector>
# include <memory>
# include <Render/Properties/IProperty.hh>

namespace AGE
{
	typedef std::vector<std::shared_ptr<IProperty>> MaterialInstance;

	struct MaterialSetInstance
	{
	public:
		std::string name;
		std::string path;
		std::vector<MaterialInstance> datas;
	};
}