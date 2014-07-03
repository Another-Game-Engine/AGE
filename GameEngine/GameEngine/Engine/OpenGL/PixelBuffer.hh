#pragma once

# include <cstdint>
# include <Utils/OpenGL.hh>

namespace gl
{
	//!\file PixelBuffer.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class PixelBuffer
	//!\brief Handle Opengl Pixel Buffer
	class PixelBuffer
	{
	public:
		virtual ~PixelBuffer();
		PixelBuffer &operator=(PixelBuffer &&p);

		virtual PixelBuffer const &bind() const = 0;
		virtual PixelBuffer const &unbind() const = 0;
		virtual PixelBuffer const &allocate(std::uint32_t size) = 0;

		GLint getId() const;

	protected:
		PixelBuffer();
		PixelBuffer(PixelBuffer &&move);

		GLuint _id;
		std::uint32_t _memorySize;
	};

	//!\file PixelBuffer.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class PixelBufferPack
	//!\brief Handle Opengl Pixel Pack Buffer
	class PixelBufferPack : public PixelBuffer
	{
	public:
		PixelBufferPack();
		virtual ~PixelBufferPack();
		PixelBufferPack(PixelBufferPack &&move);

		virtual PixelBuffer const &bind() const;
		virtual PixelBuffer const &unbind() const;
		virtual PixelBuffer const &allocate(std::uint32_t size);

	};

	//!\file PixelBuffer.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class PixelBufferUnPack
	//!\brief Handle Opengl Pixel UnPack Buffer
	class PixelBufferUnPack : public PixelBuffer
	{
	public:
		PixelBufferUnPack();
		virtual ~PixelBufferUnPack();
		PixelBufferUnPack(PixelBufferUnPack &&move);

		virtual PixelBuffer const &bind() const;
		virtual PixelBuffer const &unbind() const;
		virtual PixelBuffer const &allocate(std::uint32_t size);

	};
}