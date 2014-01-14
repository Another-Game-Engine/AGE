#ifndef   __ACONVERTOR_HPP__
# define  __ACONVERTOR_HPP__

#include <memory>
#include <string>
#include <set>
#include <Utils/File.hpp>


struct AMediaFile;
class AssetsConvertorManager;

class AConvertor
{
public:
	AConvertor(AssetsConvertorManager *manager, std::set<std::string> extensions);
	virtual ~AConvertor();
	virtual std::auto_ptr<AMediaFile> convert(const File file) = 0;
	bool supportFile(const File file);
protected:
	const std::set<std::string> _extensions;
	AssetsConvertorManager *_manager;
};

#endif    //__ACONVERTOR_HPP__