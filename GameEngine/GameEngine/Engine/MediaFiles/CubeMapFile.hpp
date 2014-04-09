#ifndef  __CUBE_MAP_FILE_HPP__
# define __CUBE_MAP_FILE_HPP__

#include <MediaFiles/MediaFile.hpp>
#include <vector>
#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/common.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/complex.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <Utils/GlmSerialization.hpp>
#include <OpenGL/VertexBuffer.hh>
#include <MediaFiles/TextureFile.hpp>


struct CubeMapFile : public MediaFile<CubeMapFile>
{
	CubeMapFile();
	virtual ~CubeMapFile();
	CubeMapFile(const CubeMapFile &o);
	CubeMapFile &operator=(const CubeMapFile &o);
	void save(cereal::PortableBinaryOutputArchive &ar) const;
	void load(cereal::PortableBinaryInputArchive &ar);
	void init();
	inline const GLuint			getId() const
	{
		return _id;
	}
	void draw() const;
	std::shared_ptr<TextureFile> px;
	std::shared_ptr<TextureFile> nx;
	std::shared_ptr<TextureFile> py;
	std::shared_ptr<TextureFile> ny;
	std::shared_ptr<TextureFile> pz;
	std::shared_ptr<TextureFile> nz;
private:
	GLuint _id;
	GLuint _vao;
	GLuint _vbo;
};

#endif   //__CUBE_MAP_FILE_HPP__
