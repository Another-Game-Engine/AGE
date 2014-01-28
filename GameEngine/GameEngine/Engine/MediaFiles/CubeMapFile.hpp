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
	CubeMapFile()
		: MediaFile<CubeMapFile>()
		, px(nullptr)
		, nx(nullptr)
		, py(nullptr)
		, ny(nullptr)
		, pz(nullptr)
		, nz(nullptr)
		, _id(0)
	{
		_type = CUBEMAP;
	}

	virtual ~CubeMapFile()
	{
		if (_id != 0)
			glDeleteTextures(1, &_id);
	}

	CubeMapFile(const CubeMapFile &o)
		: MediaFile<CubeMapFile>(o)
		, px(nullptr)
		, nx(nullptr)
		, py(nullptr)
		, ny(nullptr)
		, pz(nullptr)
		, nz(nullptr)
		, _id(0)
	{
			px = o.px;
			nx = o.nx;
			py = o.py;
			ny = o.ny;
			pz = o.pz;
			nz = o.nz;
			init();
	}

	CubeMapFile &operator=(const CubeMapFile &o)
	{
		if (&o != this)
		{
			px = o.px;
			nx = o.nx;
			py = o.py;
			ny = o.ny;
			pz = o.pz;
			nz = o.nz;
		}
		return *this;
	}


	template <typename Archive>
	void save(Archive &ar) const
	{
		ar(px->path.getFullName(), py->path.getFullName(), pz->path.getFullName(),
			nx->path.getFullName(), ny->path.getFullName(), nz->path.getFullName());
	}

	template <typename Archive>
	void load(Archive &ar)
	{
		std::string _px, _py, _pz, _nx, _ny, _nz;
		ar(_px, _py, _pz, _nx, _ny, _nz);
		px = std::static_pointer_cast<TextureFile>(AMediaFile::loadFromFile(_px));
		py = std::static_pointer_cast<TextureFile>(AMediaFile::loadFromFile(_py));
		pz = std::static_pointer_cast<TextureFile>(AMediaFile::loadFromFile(_pz));
		nx = std::static_pointer_cast<TextureFile>(AMediaFile::loadFromFile(_nx));
		ny = std::static_pointer_cast<TextureFile>(AMediaFile::loadFromFile(_ny));
		nz = std::static_pointer_cast<TextureFile>(AMediaFile::loadFromFile(_nz));
		init();
	}

	void init()
	{
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &_id);
		assert(_id > 0 && "glGenTextures error.");
		glBindTexture(GL_TEXTURE_CUBE_MAP, _id);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, px->components, px->width, px->height, 0, px->format, GL_UNSIGNED_BYTE, px->datas.data());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, nx->components, nx->width, nx->height, 0, nx->format, GL_UNSIGNED_BYTE, nx->datas.data());

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, py->components, py->width, py->height, 0, py->format, GL_UNSIGNED_BYTE, py->datas.data());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, ny->components, ny->width, ny->height, 0, ny->format, GL_UNSIGNED_BYTE, ny->datas.data());

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, pz->components, pz->width, pz->height, 0, pz->format, GL_UNSIGNED_BYTE, pz->datas.data());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, nz->components, nz->width, nz->height, 0, nz->format, GL_UNSIGNED_BYTE, nz->datas.data());

		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		// build cube
		float points[] = {
			-10.0f,  10.0f, -10.0f,
			-10.0f, -10.0f, -10.0f,
			10.0f, -10.0f, -10.0f,
			10.0f, -10.0f, -10.0f,
			10.0f,  10.0f, -10.0f,
			-10.0f,  10.0f, -10.0f,
			
			-10.0f, -10.0f,  10.0f,
			-10.0f, -10.0f, -10.0f,
			-10.0f,  10.0f, -10.0f,
			-10.0f,  10.0f, -10.0f,
			-10.0f,  10.0f,  10.0f,
			-10.0f, -10.0f,  10.0f,
			
			10.0f, -10.0f, -10.0f,
			10.0f, -10.0f,  10.0f,
			10.0f,  10.0f,  10.0f,
			10.0f,  10.0f,  10.0f,
			10.0f,  10.0f, -10.0f,
			10.0f, -10.0f, -10.0f,
			
			-10.0f, -10.0f,  10.0f,
			-10.0f,  10.0f,  10.0f,
			10.0f,  10.0f,  10.0f,
			10.0f,  10.0f,  10.0f,
			10.0f, -10.0f,  10.0f,
			-10.0f, -10.0f,  10.0f,
			
			-10.0f,  10.0f, -10.0f,
			10.0f,  10.0f, -10.0f,
			10.0f,  10.0f,  10.0f,
			10.0f,  10.0f,  10.0f,
			-10.0f,  10.0f,  10.0f,
			-10.0f,  10.0f, -10.0f,
			
			-10.0f, -10.0f, -10.0f,
			-10.0f, -10.0f,  10.0f,
			10.0f, -10.0f, -10.0f,
			10.0f, -10.0f, -10.0f,
			-10.0f, -10.0f,  10.0f,
			10.0f, -10.0f,  10.0f
		};
		unsigned int indices[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 };
		_buffer.init(36, &indices[0]);
		_buffer.addAttribute(OpenGLTools::Attribute(sizeof(float)* 3, 3, GL_FLOAT));
	}

	inline const GLuint			getId() const
	{
		return _id;
	}

	void draw() const
	{
		_buffer.draw(GL_TRIANGLES);
	}

	std::shared_ptr<TextureFile> px;
	std::shared_ptr<TextureFile> nx;
	std::shared_ptr<TextureFile> py;
	std::shared_ptr<TextureFile> ny;
	std::shared_ptr<TextureFile> pz;
	std::shared_ptr<TextureFile> nz;
private:
	GLuint _id;
	OpenGLTools::VertexBuffer _buffer;
};

#endif   //__CUBE_MAP_FILE_HPP__