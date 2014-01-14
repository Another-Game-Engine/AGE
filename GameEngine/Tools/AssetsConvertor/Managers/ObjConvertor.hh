#ifndef  __OBJ_CONVERTOR_HH__
# define __OBJ_CONVERTOR_HH__

#include <Managers\AConvertor.hh>

class AssetsConvertorManager;

class ObjConvertor : public AConvertor
{
public:
	ObjConvertor(AssetsConvertorManager *manager);
	virtual ~ObjConvertor();
private:
};

#endif  //__OBJ_CONVERTOR_HH__