#ifndef MEMORYBLOCKSGPU_H_
# define MEMORYBLOCKSGPU_H_

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
		MemoryBlocksGPU(size_t nbrBlock, size_t *startBlocks, size_t *sizeBlocks);
		MemoryBlocksGPU(MemoryBlocksGPU const &copy);
		~MemoryBlocksGPU();
		MemoryBlocksGPU &operator=(MemoryBlocksGPU const &b);

		MemoryBlocksGPU &setNbrBlock(size_t nbrBlock);
		MemoryBlocksGPU &setStartBlock(size_t index, size_t startBock);
		MemoryBlocksGPU &setSizeBlock(size_t index, size_t sizeBlock);
		size_t getNbrBlock() const;
		size_t getStartBlock(size_t index) const;
		size_t getSizeBlock(size_t index) const;

	private:
		size_t _nbrBlock;
		size_t *_startBlocks;
		size_t *_sizeBlocks;

	};
}

#endif /*!MEMORYBLOCKSGPU_H_*/