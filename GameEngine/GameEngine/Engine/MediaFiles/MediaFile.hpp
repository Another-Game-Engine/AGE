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
	}

	virtual ~MediaFile()
	{}

	MediaFile(const MediaFile &o)
		: AMediaFile(o)
	{
	}

	MediaFile &operator=(const MediaFile &o)
	{
		if (&o != this)
		{
		}
		return *this;
	}

	virtual void _serialize(cereal::PortableBinaryOutputArchive &ar)
	{
		ar(*dynamic_cast<MediaType*>(this));
	}

	virtual void _serialize(cereal::PortableBinaryInputArchive &ar)
	{
		ar(*dynamic_cast<MediaType*>(this));
	}

};

#endif    //__MEDIA_FILE_HPP__
