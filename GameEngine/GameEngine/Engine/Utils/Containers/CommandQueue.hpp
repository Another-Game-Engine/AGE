#pragma once

#include <cstdlib>
#include <new>
#include "PtrQueue.hpp"

// FOR TEST
#include <iostream>

namespace AGE
{
	namespace CommandType
	{
		struct BaseCommand
		{
			virtual void execute() = 0;
		};

		struct TestCommand : public BaseCommand
		{
			virtual void execute()
			{
				std::cout << "Exec : " << a << " n'" << b << std::endl;
			}

			char a = 'A';
			int b = 0;
			void *d = nullptr;
		};
	};

	template <typename Base = CommandType::BaseCommand>
	class CommandQueue : public PtrQueue<Base>
	{
	public:
		CommandQueue(std::size_t chunkSize = 1024)
			: PtrQueue(chunkSize)
		{
		}

		CommandQueue(CommandQueue &&o)
			: PtrQueue(std::move(o))
		{
		}

		CommandQueue&& operator=(CommandQueue &&o)
		{
			this = std::move(o);
			return std::move(*this);
		}

		virtual ~CommandQueue()
		{
		}

		void executeOne()
		{
			auto *p = pop();
			if (p != nullptr)
				p->execute();
		}

		void executeAll()
		{
			while (!isEmpty())
			{
				executeOne();
			}
		}
	};
}