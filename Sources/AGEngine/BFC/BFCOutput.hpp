#pragma once

#include <atomic>
#include <Utils/Containers/LFQueue.hpp>
#include <Utils/Containers/LFVector.hpp>

#include "BFCArray.hpp"

namespace AGE
{
	/*
	Model for RawInfos data structure
	All this method have to be implemented
	struct THIS_TYPE
	{
		static void Treat(const BFCItem &item, BFCArray<THIS_TYPE> &result);
		static bool Compare(const THIS_TYPE &a, const THIS_TYPE &b);
		static THIS_TYPE Invalid();
		bool operator!=(const THIS_TYPE &o);
	};
	*/

	template<typename CommandType>
	struct BFCCommand
	{
		inline void setKeySizeAndOffset(std::size_t size, std::size_t offset)
		{
			_size = size;
			_offset = offset;
		}
		inline std::size_t getSize() const { return _size; }
		inline std::size_t getOffset() const { return _offset; }

		template <typename RawType>
		inline void setAsCommandKey(const RawType &raw)
		{
			command.setAsCommandKey(raw);
		}

		template <typename RawType>
		inline void setAsCommandData(const RawType &raw)
		{
			command.setAsCommandData(raw);
		}
	private:
		std::size_t _size = 0;
		std::size_t _offset = 0;
	public:
		CommandType command;
	};

	struct EMPTY_BFCRawType
	{
	};

	struct EMPTY_BFCCommand
	{
		inline void setAsCommandKey(const EMPTY_BFCRawType &){}
		inline void setAsCommandData(const EMPTY_BFCRawType &){}
	};

	typedef BFCCommand<EMPTY_BFCCommand> BFCEmptyCommand;

	class IBFCOutput
	{
	public:
		IBFCOutput();
		void treatCulledChunk(const BFCCullArray &array);
		void treatCulledResult();
		void setNumberOfBlocks(const std::size_t number);

		virtual void _treatCulledChunk(const BFCCullArray &array) = 0;
		virtual void _treatCulledResult() = 0;
	protected:
		std::size_t         _dataOffset;
		std::atomic_size_t  _counter;
		LFQueue<IBFCOutput*> *_resultQueue;
	};

	template <typename RawInfosType, std::size_t RawInfosNbr
		     ,typename CommandType,  std::size_t CommandNbr>
	class BFCOutput : public IBFCOutput
	{
	public:
		struct BFCOutputChunk
		{
			BFCArray<RawInfosType> rawChunck;
			BFCOutput              *output;
		};

		virtual void _treatCulledChunk(const BFCCullArray &array)
		{
			BFCOutputChunk *chunk = nullptr;
			if (_chunckQueue->dequeue(chunk) == false)
			{
				chunk = new BFCOutputChunk();
			}

			for (ItemID i = 0; i < array.size(); ++i)
			{
				RawInfosType::Treat(array[i], chunk->rawChunck);
			}

			if (chunk->rawChunck > 0)
			{
				std::sort(chunk->rawChunck.data(), (chunk->rawChunck.data() + chunk->rawChunck.size()), RawInfosType::Compare);
			}
			mergeChunck(chunk);
		}

		virtual void _treatCulledResult()
		{
			if (_rawInfos.size() != 0)
			{
				std::size_t max = _rawInfos.size();
				std::size_t i = 0;

				CommandType *key = nullptr;
				RawInfosType lastInfos = RawInfosType::Invalid();

				std::size_t keyCounter = 0;
				std::size_t keyIndice = 0;

				std::size_t dataCounter = _commands.size() - 1;

				while (i < max && dataCounter > keyIndice)
				{
					auto &c = _rawInfos[i];
					if (c != lastInfos)
					{
						if (key)
						{
							key->setKeySizeAndOffset(keyCounter, dataCounter + 1);
						}
						keyCounter = 0;
						lastInfos = c;
						_commands[keyIndice].setAsCommandKey(c);
						key = &(_commands[keyIndice++]);
					}
					_commands[dataCounter--].setAsCommandData(_rawInfos[i]);
					++keyCounter;
					++i;
				}
				_dataOffset = dataCounter + 1;
				if (key)
				{
					key->setKeySizeAndOffset(keyCounter, dataCounter + 1);
				}
			}
		}

		void mergeChunck(BFCOutputChunk *chunck)
		{
			_rawInfos.pushChunk(chunck->rawChunck.data(), chunck->rawChunck.size());
			_chunckQueue->enqueue(chunck);
		}
	private:
		LFQueue<BFCOutputChunk*>			*_chunckQueue;
		LFVector<RawInfosType, RawInfosNbr> _rawInfos;
		std::array<CommandType, CommandNbr> _commands;
	};
}