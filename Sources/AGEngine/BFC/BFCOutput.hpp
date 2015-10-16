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

	// Example
	struct EMPTY_BFCRawType
	{
		static void Treat(const BFCItem &, BFCArray<EMPTY_BFCRawType> &){}
		static bool Compare(const EMPTY_BFCRawType &a, const EMPTY_BFCRawType &b){ return true; }
		static EMPTY_BFCRawType Invalid() { return EMPTY_BFCRawType(); }
		bool operator!=(const EMPTY_BFCRawType &o) { return true; }
	};

	// Example
	struct EMPTY_BFCCommand
	{
		inline void setAsCommandKey(const EMPTY_BFCRawType &){}
		inline void setAsCommandData(const EMPTY_BFCRawType &){}
		inline void setKeySizeAndOffset(std::size_t , std::size_t){}
	};

	typedef EMPTY_BFCCommand BFCEmptyCommand;

	class IBFCOutput
	{
	public:
		IBFCOutput();
		void treatCulledChunk(const BFCCullArray &array);
		void treatCulledResult();
		void setNumberOfBlocks(const std::size_t number);

		template <typename ResultType>
		void setResultQueue(LFQueue<ResultType*> *resultQueue)
		{
			static_assert(std::is_base_of<IBFCOutput, ResultType>::value, "Output type have to inherit from IBFCOutput");
			_resultQueue = (LFQueue<IBFCOutput*>*)(resultQueue);
		}

		virtual void _treatCulledChunk(const BFCCullArray &array) = 0;
		virtual void _treatCulledResult() = 0;
		inline std::size_t getDataOffset() const { return _dataOffset; }
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
			inline void reset() { rawChunck.clear(); }
		};

		inline void reset()
		{
			_rawInfos.clear();
			_counter = 0;
			_dataOffset = 0;
			_resultQueue = nullptr;
		}

		virtual void _treatCulledChunk(const BFCCullArray &array)
		{
			BFCOutputChunk *chunk = nullptr;
			if (_chunckQueue.try_dequeue(chunk) == false)
			{
				chunk = new BFCOutputChunk();
			}
			chunk->reset();

			for (ItemID i = 0; i < array.size(); ++i)
			{
				RawInfosType::Treat(array[i], chunk->rawChunck);
			}

			if (chunk->rawChunck.size() > 0)
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
			_chunckQueue.enqueue(chunck);
		}

		inline std::size_t getDataSize() const { return CommandNbr - getDataOffset(); }
		inline const std::array<CommandType, CommandNbr> &getCommands() const { return _commands; }

		static BFCOutput *GetNewOutput()
		{
			BFCOutput *instance = nullptr;
			if (getInstancePool().try_dequeue(instance) == false)
			{
				instance = new BFCOutput();
			}
			AGE_ASSERT(instance->_isInUse == false);
			instance->_isInUse = true;
			instance->reset();
			return instance;
		}
		static void RecycleOutput(BFCOutput *output)
		{
			AGE_ASSERT(output->_isInUse == true);
			output->_isInUse = false;
			getInstancePool().enqueue(output);
		}
	private:
		static LFQueue<BFCOutputChunk*>	    _chunckQueue;
		LFVector<RawInfosType, RawInfosNbr> _rawInfos;
		std::array<CommandType, CommandNbr> _commands;
		bool                                _isInUse = false;

		// not thread safe, but if used correctly, it should
		// be called by mainthread first, before other threads
		static LFQueue<BFCOutput*> &getInstancePool()
		{
			static LFQueue<BFCOutput*> instance;
			return instance;
		}
	};

	template <typename RawInfosType, std::size_t RawInfosNbr, typename CommandType, std::size_t CommandNbr>
	LFQueue< typename BFCOutput< RawInfosType, RawInfosNbr, CommandType, CommandNbr >::BFCOutputChunk*> BFCOutput<RawInfosType, RawInfosNbr, CommandType, CommandNbr>::_chunckQueue = LFQueue< BFCOutput< RawInfosType, RawInfosNbr, CommandType, CommandNbr >::BFCOutputChunk*>();
}