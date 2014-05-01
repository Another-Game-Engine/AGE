#include "Loaders.hh"
#include "Manager.hpp"

bool loadObj(std::tr2::sys::path file, Manager &manager)
	{
		std::vector<tinyobj::shape_t> shapes;
		std::string materialFile;
		std::string res = tinyobj::LoadObj(shapes, file.relative_path().string().c_str(), materialFile);
		auto parentPath = file.parent_path();

		if (!res.empty())
		{
			std::cerr << res << std::endl;
			return false;
		}

		manager.addFile(std::make_shared<MyMediaFile>(file.string(), file.filename(), file.extension()));

		std::tr2::sys::path mtlPath = parentPath.directory_string() + "/" + materialFile;
		manager.loadFile(std::move(mtlPath));
		return true;
	}

	bool loadMtl(std::tr2::sys::path file, Manager &manager)
	{
		std::map<std::string, tinyobj::material_t> material_map;
		std::string res = tinyobj::LoadMtl(material_map, file.relative_path().string().c_str());
		auto parentPath = file.parent_path();

		if (!res.empty())
		{
			std::cerr << res << std::endl;
			return false;
		}
		material_map.erase("");
		for (auto &&e : material_map)
		{
			manager.loadFile(std::move(parentPath.directory_string() + "/" + e.second.ambient_texname));
			manager.loadFile(std::move(parentPath.directory_string() + "/" + e.second.diffuse_texname));
			manager.loadFile(std::move(parentPath.directory_string() + "/" + e.second.normal_texname));
			manager.loadFile(std::move(parentPath.directory_string() + "/" + e.second.specular_texname));
		}
		manager.addFile(std::make_shared<MyMediaFile>(file.string(), file.filename(), file.extension()));
		return true;
	}

	bool loadTga(std::tr2::sys::path file, Manager &manager)
	{
		manager.addFile(std::make_shared<MyMediaFile>(file.string(), file.filename(), file.extension()));
		return true;
	}