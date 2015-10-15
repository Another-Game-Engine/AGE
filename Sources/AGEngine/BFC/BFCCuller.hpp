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

	namespace BFCPrivate
	{
		class CullerBase
		{
		public:
			inline const BFCCullArray      &getArray() const { return _cullerArray; }
		protected:
			BFCCullArray      _cullerArray;
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