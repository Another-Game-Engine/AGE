#ifdef TEXTURE_HH_

namespace OpenGLTools
{
	template <typename TYPE>
	Texture<TYPE>::Texture()
	{
	}

	template <>
	Texture<texture2D>::Texture()
	{
		bind();
//		glTexImage2D(static_cast<GLenum>(texture2D::type), 0, internalFormat, width, height, 0, format, type, NULL);
	}

	template <typename TYPE>
	Texture<TYPE>::~Texture()
	{
		glDeleteTextures(1, _id);
	}

	template <typename TYPE>
	GLuint Texture<TYPE>::getId() const
	{
		return (_id);
	}

	template <typename TYPE>
	void Texture<TYPE>::bind() const
	{
//		glBindTexture(TYPE::type, _id);
	}

	template <typename TYPE>
	void Texture<TYPE>::unbind() const
	{
	//	glBindTexture(TYPE::type, 0);
	}
}

#endif /*!TEXTURE_HPP_*/