#include "CubeMap.hh"

#include "Parsers/Parsers.hh"
#include "Utils/OpenGL.hh"
#include "Utils/File.hpp"

namespace	Resources
{

	CubeMap::CubeMap(void)
		: _id(0)
		, _vbo(0)
		, _vao(0)
	{
	}


	CubeMap::~CubeMap(void)
	{
		if (_id != 0)
			glDeleteTextures(1, &_id);
		if (_vao != 0)
			glDeleteVertexArrays(1, &_vao);
		if (_vbo != 0)
			glDeleteBuffers(1, &_vbo);
	}

	bool			CubeMap::load(std::string const &path)
	{
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, _id);
		const char *suffixes[] = {"posx", "negx", "negy", "posy", "posz", "negz"};
		GLuint targets[] = {
			GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
		};

		for (unsigned int i = 0; i < 6; ++i)
		{
			File file(path + "/" + suffixes[i] + ".tga");
			if (!file.exists())
				return false; // TODO -> erase texture
			GLbyte *datas;
			GLint width, height;
			GLint components;
			GLenum format;
			if ((datas = loadTGA(file.getFullName().c_str(), &width, &height, &components, &format)) == NULL)
				return false; // TODO -> erase texture
			glTexImage2D(targets[i], 0, components, width, height, 0, format, GL_UNSIGNED_BYTE, datas);
		}

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
		glBindBuffer (GL_ARRAY_BUFFER, _vbo);
		glBufferData (GL_ARRAY_BUFFER, 3 * 36 * sizeof (float), &points, GL_STATIC_DRAW);
		
		glGenVertexArrays (1, &_vao);
		glBindVertexArray (_vao);
		glEnableVertexAttribArray (0);
		glBindBuffer (GL_ARRAY_BUFFER, _vbo);
		glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
		return true;
	}

	const GLuint			CubeMap::getId() const
	{
		return (_id);
	}

	void CubeMap::draw() const
	{
		glBindVertexArray (_vao);
		glDrawArrays (GL_TRIANGLES, 0, 36);
	}
}
