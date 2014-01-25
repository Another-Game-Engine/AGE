#ifndef   __ASSETS_MANAGER_HPP__
# define  __ASSETS_MANAGER_HPP__

#include <Utils/Dependency.hpp>
#include <MediaFiles/AMediaFile.hpp>
#include <memory>

class AssetsManager : public Dependency
{
public:
	AssetsManager()
	{}

	virtual ~AssetsManager()
	{}

	void registrarLoadedAsset(const std::string &name, std::shared_ptr<AMediaFile> file)
	{
		_ref.insert(std::make_pair(name, file));
	}

	void assetHandle(std::string &name, std::shared_ptr<AMediaFile> *file)
	{
		_toUpdate.insert(std::make_pair(name, file));
	}

	void updateAssetHandles()
	{
		for (auto &e : _ref)
		{
			updateAssetHandle(e.first, e.second);
		}
		assert(_toUpdate.size() == 0 && "All handles have not been unserialized correctly.");
	}

	void updateAssetHandle(std::string name, std::shared_ptr<AMediaFile> r)
	{
		auto ret = _toUpdate.equal_range(name);
		for (auto it = ret.first; it != ret.second; ++it)
		{
			*(it->second) = r;
		}
		_toUpdate.erase(name);
	}
protected:
	std::map<std::size_t, AMediaFile*> refs;
	std::map<std::string, std::shared_ptr<AMediaFile>> _ref;
	std::multimap<std::string, std::shared_ptr<AMediaFile> *> _toUpdate;
};

#endif    //__ASSETS_MANAGER_HPP__