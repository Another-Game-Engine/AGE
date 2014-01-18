#include <MediaFiles/AMediaFile.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/polymorphic.hpp>

AMediaFile::AMediaFile()
{
}

AMediaFile::~AMediaFile()
{}

//CEREAL_REGISTER_TYPE(AMediaFile);