#ifndef MEMORYBLOCKSGPU_H_
# define MEMORYBLOCKSGPU_H_

#include <cstdint>

namespace gl
{
	//!\file MemoryBlocksGPU.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class MemoryBlocksGPU
	//!\brief Representing data on GPU
	class MemoryBlocksGPU
	{
	public:
		MemoryBlocksGPU();
		MemoryBlocksGPU(size_t nbrElement, size_t nbrBlock, size_t *startBlocks, size_t *sizeBlocks);
		MemoryBlocksGPU(MemoryBlocksGPU const &copy);
		~MemoryBlocksGPU();
		MemoryBlocksGPU &operator=(MemoryBlocksGPU const &b);

		MemoryBlocksGPU &setSync(bool sync);
		MemoryBlocksGPU &setStartElement(size_t start);
		MemoryBlocksGPU &setNbrElement(size_t nbrElement);
		MemoryBlocksGPU &setNbrBlock(size_t nbrBlock);
		MemoryBlocksGPU &setSizeBlock(uint8_t index, size_t sizeBlock);
		MemoryBlocksGPU &setOffset(uint8_t index, size_t offset);
		bool getSync() const;
		size_t getElementStart() const;
		size_t getNbrElement() const;
		size_t getNbrBlock() const;
		size_t getSizeBlock(uint8_t index) const;
		size_t getOffset(uint8_t index) const;


	private:
		size_t _nbrElement;
		size_t _startElement;
		size_t _nbrBlock;
		size_t *_sizeBlocks;
		size_t *_baseOffset;
		bool _sync;

	};
}

#endif /*!MEMORYBLOCKSGPU_H_*/