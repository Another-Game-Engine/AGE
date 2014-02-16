#ifndef   __ASSETS_MANAGER_HPP__
# define  __ASSETS_MANAGER_HPP__

#include <Utils/Dependency.hpp>
#include <Utils/File.hpp>
#include <memory>
#include <map>

struct AMediaFile;

class AssetsManager : public Dependency, public std::enable_shared_from_this<AssetsManager>
{
public:
	AssetsManager();
	virtual ~AssetsManager();
	void add(std::shared_ptr<AMediaFile> f);
	std::shared_ptr<AMediaFile> get(const File &file);
	std::shared_ptr<AMediaFile> get(const std::string &name);
protected:
	std::map<std::string, std::shared_ptr<AMediaFile>> _files;
};

#endif    //__ASSETS_MANAGER_HPP__