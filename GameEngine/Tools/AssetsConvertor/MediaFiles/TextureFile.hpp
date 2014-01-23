#ifndef  __TEXTURE_FILE_HPP__
# define __TEXTURE_FILE_HPP__

#include <MediaFiles/MediaFile.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/common.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/complex.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <Utils/GlmSerialization.hpp>
#include <OpenGL/include/SDL/SDL_opengl.h>

struct TextureFile : public MediaFile<TextureFile>
{
	TextureFile() : MediaFile<TextureFile>()
	{
	}
	virtual ~TextureFile(){}

	GLbyte *bytes;

	template <typename Archive>
	AMediaFile *unserialize(Archive &ar)
	{
		AMediaFile *res = new TextureFile();
		ar(static_cast<TextureFile&>(*res));
		return res;
	}

	template <typename Archive>
	void serialize(Archive &ar)
	{
	}
};

#endif   //__TEXTURE_FILE_HPP__