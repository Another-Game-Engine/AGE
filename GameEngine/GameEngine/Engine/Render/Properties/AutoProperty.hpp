#pragma once

#include <Render/Properties/AProperty.hh>

namespace AGE
{
	// Be very carefull with that type of property
	// It's a lot easiest to configure but, it's can
	// generate indefined behaviour if you pass the wrong type
	// there is no verification in the AutoSet function !!!
	template <typename DataType, typename UpdateType>
	class AutoProperty : public AProperty
	{
	public:
		AutoProperty(std::string &&name)
			: AProperty(std::move(name))
		{
		}

		const DataType &getValue()
		{
			std::lock_guard<AGE::SpinLock> lock(_mutex);
			return _data;
		}

		virtual ~AutoProperty()
		{
		}

		void set(const DataType &value)
		{
			std::lock_guard<AGE::SpinLock> lock(_mutex);
			_data = value;
		}

		static void update(UpdateType* res, AutoProperty*p)
		{
			*res = p->_data;
		}

		static inline void _updateFunctionLock(IProgramResources *r, IProperty*p)
		{
			p->_mutex.lock(); AutoProperty::update((UpdateType*)(r), (AutoProperty*)(p)); p->_mutex.unlock();
		}
		inline virtual void(*getUpdateFunction())(IProgramResources *, IProperty*)
		{
			return AutoProperty::_updateFunctionLock;
		}

		static void instanciedUpdate(UpdateType*, AutoProperty*)
		{
			assert(false);
		}

		static inline void _instanciedUpdateFunctionLock(IProgramResources *r, IProperty*p)
		{
			p->_mutex.lock(); AutoProperty::instanciedUpdate((UpdateType*)(r), (AutoProperty*)(p)); p->_mutex.unlock();
		}
		inline virtual void(*getInstanciedUpdateFunction())(IProgramResources *, IProperty*)
		{	
			return AutoProperty::_instanciedUpdateFunctionLock;
		}

	private:
		virtual void _autoSet(void *dataPtr)
		{
			// totally unsafe, you have to be sure of what you're doing
			std::lock_guard<AGE::SpinLock> lock(_mutex);
			auto newData = (DataType*)(dataPtr);
			_data = *newData;
		}

		DataType _data;
	};
}