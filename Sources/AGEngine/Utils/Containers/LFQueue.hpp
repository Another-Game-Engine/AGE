#pragma once

#include <concurrentqueue/concurrentqueue.h>

namespace AGE
{
	template <typename T>
	class LFQueue : public moodycamel::ConcurrentQueue<T>
	{

	};
}