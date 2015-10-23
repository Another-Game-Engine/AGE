#pragma once

#include <TMQ/message.hpp>

using namespace TMQ;

namespace AGE
{
	struct ICallbackContainer
	{
		virtual void operator()(TMQ::MessageBase *m) = 0;
		virtual ~ICallbackContainer();
		ICallbackContainer();
		inline bool isValid() { return _isValid; }
	protected:
		bool _isValid;
	};

	template <typename T>
	struct CallbackContainer : public ICallbackContainer
	{
		CallbackContainer()
		{
		}

		CallbackContainer(const std::function<void(T &)> &fn)
			: function(fn)
		{
			_isValid = true;
		}
		virtual void operator()(TMQ::MessageBase *m)
		{
			assert(isValid());
			auto message = static_cast<TMQ::Message<T>*>(m);
			T copy = message->_data;
			message->~Message<T>();
			function(copy);
		}
		virtual ~CallbackContainer(){}

		std::function<void(T &)> function;
	};
}