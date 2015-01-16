#pragma once

# include <string>
# include <vector>
# include <memory>
# include <Render/Properties/IProperty.hh>

struct MaterialSetInstance
{
public:
	std::string name;
	std::string path;
	std::vector<std::shared_ptr<IProperty>> datas;
};