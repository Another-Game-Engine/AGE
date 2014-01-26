#ifndef  __CUBE_MAP_HPP__
# define __CUBE_MAP_HPP__

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
#include <Utils/OpenGL.hh>
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
		, _vbo(0)
		, _vao(0)
	{
		_type = CUBEMAP;
	}

	virtual ~CubeMapFile()
	{
		if (_id != 0)
			glDeleteTextures(1, &_id);
		if (_vao != 0)
			glDeleteVertexArrays(1, &_vao);
		if (_vbo != 0)
			glDeleteBuffers(1, &_vbo);
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

		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &_id);
		assert(_id > 0 && "glGenTextures error.");
		glBindTexture(GL_TEXTURE_CUBE_MAP, _id);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, px->components, px->width, px->height, 0, px->format, GL_UNSIGNED_BYTE, px->datas);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, nx->components, nx->width, nx->height, 0, nx->format, GL_UNSIGNED_BYTE, nx->datas);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, py->components, py->width, py->height, 0, py->format, GL_UNSIGNED_BYTE, py->datas);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, ny->components, ny->width, ny->height, 0, ny->format, GL_UNSIGNED_BYTE, ny->datas);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, pz->components, pz->width, pz->height, 0, pz->format, GL_UNSIGNED_BYTE, pz->datas);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, nz->components, nz->width, nz->height, 0, nz->format, GL_UNSIGNED_BYTE, nz->datas);

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

		glGenBuffers (1, &_vbo);
		assert(_vbo > 0 && "glGenBuffers error.");
		glBindBuffer (GL_ARRAY_BUFFER, _vbo);
		glBufferData (GL_ARRAY_BUFFER, 3 * 36 * sizeof (float), &points, GL_STATIC_DRAW);
		
		glGenVertexArrays (1, &_vao);
		assert(_vao > 0 && "glGenVertexArrays error.");
		glBindVertexArray (_vao);
		glEnableVertexAttribArray (0);
		glBindBuffer (GL_ARRAY_BUFFER, _vbo);
		glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	}

	inline const GLuint			getId() const
	{
		return _id;
	}

	void draw() const
	{
		glBindVertexArray (_vao);
		glDrawArrays (GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

	std::shared_ptr<TextureFile> px;
	std::shared_ptr<TextureFile> nx;
	std::shared_ptr<TextureFile> py;
	std::shared_ptr<TextureFile> ny;
	std::shared_ptr<TextureFile> pz;
	std::shared_ptr<TextureFile> nz;
private:
	GLuint _id;
	GLuint _vbo;
	GLuint _vao;
};

#endif   //__CUBE_MAP_HPP__