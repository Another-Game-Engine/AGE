#pragma once

#include "queue.hpp"

namespace TMQ
{
	struct MessageBase;

	class Dispatcher
	{
	private:
		TMQ::Queue *_queue;
		bool _chained;

		Dispatcher(const Dispatcher &) = delete;
		Dispatcher &operator=(const Dispatcher &) = delete;

		template < typename _Dispatcher
			, typename _Message
			, typename _Func>
			friend class TemplateDispatcher;

		void waitAndDispatch();
		bool dispatch(MessageBase* &msg);
	public:
		Dispatcher(Dispatcher &&o);
		explicit Dispatcher(TMQ::Queue *queue);

		template < typename _Message
			, typename _Func>
			TemplateDispatcher<Dispatcher, _Message, _Func>
			handle(_Func &&f)
		{
			return TemplateDispatcher<Dispatcher, _Message, _Func>(_queue, this, std::forward<_Func>(f));
		}
		~Dispatcher();
	};
}