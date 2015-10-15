#pragma once

///////////////////////
// A Culler is the global helper who will 
// establish the communication between BFCCUllable items
// and render pass
// It exists multiple type of cullers : Frustum, Sphere, Box etc
// You can add them render pass outputs, specify their culling channels,
// and specialize their render preparation jobs

namespace AGE
{
	class IBFCOutput
	{
	public:
		void treatCulledChunk(const BFCCullVector &array)
		{
			_treatCulledChunk(array);
			std::size_t count = _counter.fetch_sub(1);
			if (count == 0)
			{
				treatCulledResult();
			}
		}
		void treatCulledResult()
		{
			_treatCulledResult();
			_resultQueue->enqueue(this);
		}
		void setNumberOfBlocks(const std::size_t number)
		{
			_counter = number;
		}
		virtual void _treatCulledChunk(const BFCCullVector &array) = 0;
		virtual void _treatCulledResult() = 0;
	protected:
		std::size_t         _dataOffset;
		std::atomic_size_t  _counter;
		LFQueue<IBFCOutput*> *_resultQueue;
	};

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
	struct BFCCommandBase
	{
		inline void setKeySizeAndOffset(std::size_t size, std::size_t offset)
		{
			_size = size;
			_offset = offset;
		}
		inline std::size_t getSize() const { return _size; }
		inline std::size_t getOffset() const { return _offset; }
		template <typename RawType>
		inline void setAsCommand(const RawType &raw)
		{
			command.setAsKey(raw);
		}
		inline void setAsData(const RawType &raw)
		{
			command.setAsData(raw);
		}
	private:
		std::size_t _size = 0;
		std::size_t _offset = 0;
	public:
		CommandType command;
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

		virtual void _treatCulledChunk(const BFCCullVector &array)
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
						_commands[keyIndice].setAsKey(c); //todo
						key = &(_commands[keyIndice++]);
					}
					_commands[dataCounter--].setAsDatas(_rawInfos[i]); //todo	
					++keyCounter;
					++i;
				}
				_dataOffset = dataCounter + 1;
				if (key)
				{
					key->setKeySizeAndOffset(keyCounter, dataCounter + 1); //todo
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

	class
	// TODO
	// specialize output for non-skinned shadow mesh, and basic deferred render mesh


	// will treat culling results
	// for render pass (BFCCullingOutput)
	class BFCCullingProcessor;

	template <typename CullerType>
	class BFCCuller
	{
	public:
		BFCCuller()
		{}

		~BFCCuller()
		{}

		// call that each frame
		// before each culling operation
		template<typename ...Args>
		inline void prepareForCulling(Args... args)
		{
			_culler.prepareForCulling(args...);
			_counter = 0;
		}

		template<typename OutputType>
		void addOutput(BFCCullableType cullingChannel, OutputType *output)
		{
			auto find = _channels.find(cullingChannel);
			if (find == std::end(_channels))
			{
				_channels.insert(std::make_pair(cullingChannel, std::vector<IBFCOutput*>()))
			}
			_channels[cullingChannel].push_back(output);
		}

		std::atomic_size_t *cull(BFCBlockManagerFactory *factory)
		{
			for (auto &channel : _channels)
			{
				auto blockNumber = factory->getBlockNumberToCull(channel.first);
				_counter.fetch_add(blockNumber);

				for (IBFCOutput *output : channel.second)
				{
					output->setNumberOfBlocks(blockNumber);
				}
				for (std::size_t i = 0; i < blockNumber; ++i)
				{
					TaskManager::emplaceSharedTask<Tasks::Basic::VoidFunction>([factory, i, &channel, &_culler, &_counter]()
					{
						factory->cullOnBlock(channel.first, _culler, i, 1, channel.second);
						_counter.fetch_sub(1);
					});
				}
			}
			return _counter;
		}

	private:
		CullerType                               _culler;
		std::map<BFCCullableType, std::vector<IBFCOutput>> _channels;
		std::atomic_size_t                       _counter;
	};

	// Cullers
	//////////////////////////////////////////

	typedef BFCArray<BFCItem> BFCCullVector;

	namespace BFCPrivate
	{
		class CullerBase
		{
		public:
			inline const BFCCullVector      &getArray() const { return _cullerArray; }
		protected:
			BFCCullVector      _cullerArray;
		};
	}

	class BFCFrustumCuller : public BFCPrivate::CullerBase
	{
	public:
		void prepareForCulling(const Frustum &frustum)
		{
			_frustum = frustum;
		}
		inline void cullItem(const BFCItem &item)
		{
			if (item.getDrawable() && _frustum.checkCollision(item.getPosition()))
			{
				_cullerArray.push(item);
			}
		}

		void
	private:
		Frustum            _frustum;
	};
}