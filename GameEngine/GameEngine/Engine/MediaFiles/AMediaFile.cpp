#include <MediaFiles/AMediaFile.hpp>
#include <MediaFiles/MaterialFile.hpp>
#include <MediaFiles/ObjFile.hpp>
#include <MediaFiles/TextureFile.hpp>
#include <MediaFiles/CubeMapFile.hpp>
#include <MediaFiles/CollisionShapeStaticFile.hpp>
#include <MediaFiles/CollisionShapeDynamicFile.hpp>
#include <MediaFiles/CollisionBoxFile.hpp>
#include <MediaFiles/CollisionSphereFile.hpp>
#include <MediaFiles/AssetsManager.hpp>


#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/complex.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>

#include <memory>

AMediaFile::AMediaFile() :
_childs(0)
, _type(UNKNOWN)
{
}

AMediaFile::~AMediaFile(){}

AMediaFile::AMediaFile(const AMediaFile &o)
: _childs(0)
{
	_type = o._type;
	name = o.name;
	name += "_copy";
	auto tmpPath = o.path.getFolder() + o.path.getShortFileName();
	tmpPath += "_copy.cpd";
	path = File(tmpPath);
	_dpyManager = o._dpyManager;
}

AMediaFile &AMediaFile::operator=(const AMediaFile &o)
{
	if (&o != this)
	{
		_type = o._type;
		name = o.name;
		name += "_copy";
		_dpyManager = o._dpyManager;
	}
	return *this;
}

void AMediaFile::serializeAsBulletFile(std::ofstream &s)
{
	if (_type == COLLISION_SHAPE_STATIC)
	{
		static_cast<CollisionShapeStaticFile*>(this)->serialize(s);
	}
	else if (_type == COLLISION_SHAPE_DYNAMIC)
	{
		static_cast<CollisionShapeDynamicFile*>(this)->serialize(s);
	}
	else if (_type == COLLISION_BOX)
	{
		static_cast<CollisionBoxFile*>(this)->serialize(s);
	}
	else if (_type == COLLISION_SPHERE)
	{
		static_cast<CollisionSphereFile*>(this)->serialize(s);
	}
}

	void AMediaFile::saveToFile()
	{
		std::ofstream ofs(this->path.getFullName().c_str(), std::ios::binary);
		assert(ofs.is_open() && "Cannot open file for save");
		serialize(ofs);
	}


	void AMediaFile::serialize(std::ofstream &s)
	{
		if (_type >= COLLISION_SHAPE_STATIC && _type <= COLLISION_MULTI_SPHERE)
		{
			serializeAsBulletFile(s);
			return;
		}
		else
		{
			cereal::PortableBinaryOutputArchive ar(s);
			ar(_type);
			_serialize(ar);
		}
	}