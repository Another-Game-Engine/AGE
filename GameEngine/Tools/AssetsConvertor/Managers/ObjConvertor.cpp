#include <Managers/ObjConvertor.hh>
#include <Managers/AssetsConvertorManager.hh>

ObjConvertor::ObjConvertor(AssetsConvertorManager *manager)
: AConvertor(manager, std::set<std::string>({ ".obj" }))
{}

ObjConvertor::~ObjConvertor()
{}
