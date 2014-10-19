#pragma once

#include <stdint.h>
#include <Utils/Containers/Vector.hpp>

namespace gl
{
	template<typename TYPE> class Key;

	class Data
	{
	public:
		Data();
		Data(size_t nbrElement, AGE::Vector<size_t> const &sizeBuffers, AGE::Vector<void *> const &buffers);
		Data(size_t nbrElement, AGE::Vector<uint32_t> const &buffer);
		~Data();
		Data(Data const &copy);
		Data &operator=(Data const &v);

	public:
		void const *getBuffer(uint8_t index) const;
		size_t getSizeBuffer(uint8_t index) const;
		size_t getNbrBuffers() const;
		size_t getNbrElement() const;

	private:
		AGE::Vector<void *> _buffers;
		AGE::Vector<size_t> _sizeBuffers;
		size_t _nbrElement;
	
	private:
		void copyData(AGE::Vector<void *> const &data, AGE::Vector<size_t> const &sizeData);
		void cleanData();
	};

}