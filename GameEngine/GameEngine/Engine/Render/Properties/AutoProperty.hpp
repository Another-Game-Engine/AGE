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
		{}

		void set(const DataType &value)
		{
			std::lock_guard<AGE::SpinLock> lock(_mutex);
			_data = value;
		}

	private:
		virtual void _update(std::shared_ptr<Program> const &p) override final
		{
			auto resource = std::static_pointer_cast<UpdateType>(get_resource(p));
			if (resource)
			{
				*resource = _data;
			}
		}

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