#include <MediaFiles/ObjFile.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/polymorphic.hpp>

ObjFile::ObjFile()
{}

ObjFile::~ObjFile()
{}

//CEREAL_REGISTER_TYPE(ObjFile);
//CEREAL_REGISTER_TYPE(AMediaFileBase<ObjFile>);