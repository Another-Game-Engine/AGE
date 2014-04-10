#ifndef TEXTURE_HH_
# define TEXTURE_HH_

# include "Utils/OpenGL.hh"

namespace OpenGLTools
{
	enum class texture1D
	{
		type = GL_TEXTURE_1D
	};

	enum class texture1DArray
	{
		type = GL_TEXTURE_1D_ARRAY
	};

	enum class texture2D
	{
		type = GL_TEXTURE_2D
	};

	enum class texture2DArray
	{
		type = GL_TEXTURE_2D_ARRAY
	};

	enum class texture2DMS
	{
		type = GL_TEXTURE_2D_MULTISAMPLE
	};

	enum class texture2DMSArray
	{
		type = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
	};

	enum class texture3D
	{
		type = GL_TEXTURE_3D
	};

	enum class textureCube
	{
		type = GL_TEXTURE_CUBE_MAP
	};

	enum class textureCubeArray
	{
		type = GL_TEXTURE_CUBE_MAP_ARRAY
	};

	enum class textureRect
	{
		type = GL_TEXTURE_RECTANGLE
	};

	enum class textureBuffer
	{
		type = GL_BUFFER
	};

	template <typename TYPE>
	class Texture
	{
	private:
		GLuint _id;
		
	public:
		Texture();
		Texture(size_t idnex, size_t fofo, GLenum foie, GLenum gop);
		~Texture();
		inline void bind() const;
		inline void unbind() const;
		inline GLuint getId() const;
	};
}
# include "OpenGL/Texture.hpp"

#endif /*!TEXTURE_HH_*/