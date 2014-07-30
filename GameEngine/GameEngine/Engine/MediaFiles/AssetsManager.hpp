#ifndef   __ASSETS_MANAGER_HPP__
# define  __ASSETS_MANAGER_HPP__

#include <Utils/Dependency.hpp>
#include <Utils/File.hpp>
#include <memory>
#include <map>

#include <cereal/cereal.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/complex.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/portable_binary.hpp>

struct AMediaFile;
class Engine;
namespace gl { class ShadingManager; }

class AssetsManager : public std::enable_shared_from_this<AssetsManager>, public Dependency<AssetsManager>
{
public:
	AssetsManager();
	virtual ~AssetsManager();
	virtual void init();
	void add(std::shared_ptr<AMediaFile> f);

	template <typename T = AMediaFile>
	std::shared_ptr<T> getFromFile(const File &file)
	{
		std::string name = file.getShortFileName();
		auto it = _files.find(name);
		if (it == std::end(_files))
			return std::static_pointer_cast<T>(loadFromFile(file));
		return std::static_pointer_cast<T>(it->second);
	}

	template <typename T = AMediaFile>
	std::shared_ptr<T> get(const std::string &name)
	{
		auto it = _files.find(name);
		if (it == std::end(_files))
			return std::shared_ptr<T>{nullptr};
		return std::static_pointer_cast<T>(it->second);
	}

	std::shared_ptr<AMediaFile> loadFromFile(const File &file, gl::ShadingManager &manager);
	void loadFromList(const File &file, gl::ShadingManager &manager);
	std::shared_ptr<AMediaFile> loadBulletFile(const File &file);

	template <class T>
	std::shared_ptr<T> create(const std::string &name = "", std::shared_ptr<AMediaFile> copyFrom = nullptr)
	{
		if (!name.empty())
		{
			auto o = std::static_pointer_cast<T>(get(name));
			if (o != nullptr)
				return o;
		}
		std::shared_ptr<T> n{ nullptr };
		if (copyFrom != nullptr)
			n = std::make_shared<T>(*std::static_pointer_cast<const T>(copyFrom).get());
		else
			n = std::make_shared<T>();
		if (!name.empty())
		{
			n->name = name;
			n->path = File(n->path.getFolder() + name + "." + n->path.getExtension());
		}
		add(n);
		return n;
	}

protected:
	std::map<std::string, std::shared_ptr<AMediaFile>> _files;
};

#endif    //__ASSETS_MANAGER_HPP__
