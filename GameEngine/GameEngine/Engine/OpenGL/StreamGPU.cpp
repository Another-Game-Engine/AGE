#include <OpenGL/StreamGPU.hh>

namespace OpenGLTools
{
	StreamGPU::StreamGPU()
		: _currentPBOPack(0),
		_currentPBOUnPack(0)
	{

	}

	StreamGPU::~StreamGPU()
	{

	}

	StreamGPU::StreamGPU(StreamGPU &&move)
		: _pboPack(std::move(move._pboPack)),
		_pboUnPack(std::move(move._pboUnPack)),
		_currentPBOPack(std::move(_currentPBOPack)),
		_currentPBOUnPack(std::move(_currentPBOUnPack))
	{

	}

	StreamGPU &StreamGPU::operator=(StreamGPU &&move)
	{
		_pboPack = std::move(move._pboPack);
		_pboUnPack = std::move(move._pboUnPack);
		_currentPBOPack = std::move(_currentPBOPack);
		_currentPBOUnPack = std::move(_currentPBOUnPack);
		return (*this);
	}

	StreamGPU &StreamGPU::upload(void const *data, Texture const *texture, uint32_t size)
	{
		_pboUnPack[_currentPBOUnPack].bind();
		GLubyte *ptr = (GLubyte *)glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, (GLsizeiptr)size, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
		memcpy(ptr, data, size);
		glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
		_pboUnPack[1 - _currentPBOUnPack].bind();
		texture->bind();
		texture->write(0);
		_currentPBOUnPack = 1 - _currentPBOUnPack;
		return (*this);
	}

	void *StreamGPU::beginReadBack(Texture const *texture, uint32_t size)
	{
		texture->bind();
		_pboPack[_currentPBOPack].bind();
		_pboPack[_currentPBOPack].allocate(size);
		texture->read(0);
		_pboPack[1 - _currentPBOPack].bind();
		void *ptr = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
		return (ptr);
	}

	StreamGPU &StreamGPU::endReadBack(void *data)
	{
		if (data)
			glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
		_currentPBOPack = 1 - _currentPBOPack;
		return (*this);
	}
}