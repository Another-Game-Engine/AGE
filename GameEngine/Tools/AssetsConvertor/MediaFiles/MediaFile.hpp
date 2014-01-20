#ifndef   __MEDIA_FILE_HPP__
# define  __MEDIA_FILE_HPP__

#include <MediaFiles/AMediaFile.hpp>

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/portable_binary.hpp>

template <class MediaType>
struct MediaFile : public AMediaFile
{
	MediaFile()
	{
		type = typeid(MediaType).hash_code();
	}

	virtual ~MediaFile()
	{}

	virtual AMediaFile *unserialize(cereal::JSONInputArchive &ar)
	{
		return dynamic_cast<MediaType*>(this)->unserialize<cereal::JSONInputArchive>(ar);
	}

	virtual AMediaFile *unserialize(cereal::BinaryInputArchive &ar)
	{
		return dynamic_cast<MediaType*>(this)->unserialize<cereal::BinaryInputArchive>(ar);
	}

	virtual AMediaFile *unserialize(cereal::XMLInputArchive &ar)
	{
		return dynamic_cast<MediaType*>(this)->unserialize<cereal::XMLInputArchive>(ar);
	}

	virtual AMediaFile *unserialize(cereal::PortableBinaryInputArchive &ar)
	{
		return dynamic_cast<MediaType*>(this)->unserialize<cereal::PortableBinaryInputArchive>(ar);
	}

	virtual void _serialize(cereal::JSONOutputArchive &ar)
	{
		ar(*dynamic_cast<MediaType*>(this));
	}

	virtual void _serialize(cereal::BinaryOutputArchive &ar)
	{
		ar(*dynamic_cast<MediaType*>(this));
	}

	virtual void _serialize(cereal::XMLOutputArchive &ar)
	{
		ar(*dynamic_cast<MediaType*>(this));
	}

	virtual void _serialize(cereal::PortableBinaryOutputArchive &ar)
	{
		ar(*dynamic_cast<MediaType*>(this));
	}
};

#endif    //__MEDIA_FILE_HPP__