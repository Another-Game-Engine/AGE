#pragma once

#include <TMQ/message.hpp>

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
			function(static_cast<TMQ::Message<T>*>(m)->_data);
		}
		virtual ~CallbackContainer(){}

		std::function<void(T &)> function;
	};
}