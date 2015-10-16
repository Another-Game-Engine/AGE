#pragma once

///////////////////////
// A Culler is the global helper who will 
// establish the communication between BFCCUllable items
// and render pass
// It exists multiple type of cullers : Frustum, Sphere, Box etc
// You can add them render pass outputs, specify their culling channels,
// and specialize their render preparation jobs

#include <vector>
#include <map>

#include "TMQ/Queue.hpp"
#include "Threads/Tasks/BasicTasks.hpp"

namespace AGE
{
	class IBFCOutput;

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
		void addOutput(CullableTypeID cullingChannel, OutputType *output)
		{
			auto find = _channels.find(cullingChannel);
			if (find == std::end(_channels))
			{
				_channels.insert(std::make_pair(cullingChannel, std::vector<IBFCOutput*>()));
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
					TMQ::TaskManager::emplaceSharedTask<Tasks::Basic::VoidFunction>([factory, i, &channel, this]()
					{
						// TODO make a global pool of culler
						CullerType *culler = BFCCullerMethod<CullerType>::GetNewCullerMethod();
						*culler = _culler;
						factory->cullOnBlock(channel.first, *culler, i, 1, channel.second);
						_counter.fetch_sub(1);
						BFCCullerMethod<CullerType>::Recycle(culler);
					});
				}
			}
			return &_counter;
		}

	private:
		CullerType                               _culler;
		std::map<CullableTypeID, std::vector<IBFCOutput*>> _channels;
		std::atomic_size_t                       _counter;
	};

	// Cullers
	//////////////////////////////////////////

	template <typename T>
	class BFCCullerMethod
	{
	public:
		inline const BFCCullArray      &getArray() const { return _cullerArray; }
		inline void                    reset() { _cullerArray.clear(); }
		static T                       *GetNewCullerMethod()
		{
			T *t;
			if (_pool.try_dequeue(t) == false)
			{
				t = new T();
			}
			t->reset();
			return t;
		}
		static void                   Recycle(T *t)
		{
			_pool.enqueue(t);
		}
		BFCCullerMethod &operator=(const BFCCullerMethod &o)
		{
			// do nothing, we don't want to copy array
			return *this;
		}
	protected:
		BFCCullArray      _cullerArray;
		static LFQueue<T*> _pool;
	};

	template <typename T>
	LFQueue<T*> BFCCullerMethod<T>::_pool = LFQueue<T*>();
}