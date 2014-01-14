#ifndef  __OBJ_CONVERTOR_HH__
# define __OBJ_CONVERTOR_HH__

#include <Managers\AConvertor.hh>

class AssetsConvertorManager;

class ObjConvertor : public AConvertor
{
public:
	ObjConvertor(AssetsConvertorManager *manager);
	virtual ~ObjConvertor();
	virtual std::auto_ptr<AMediaFile> convert(const File file);
private:
};

#endif  //__OBJ_CONVERTOR_HH__