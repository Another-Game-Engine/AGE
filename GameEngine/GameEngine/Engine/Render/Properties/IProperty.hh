#pragma once

#include <string>
#include <memory>
#include <vector>
#include <Render/ProgramResources/IProgramResources.hh>
#include <Utils/SpinLock.hpp>
#include <mutex>
#include<Utils/Debug.hpp>

namespace AGE
{
	class IProgramResources;

	class IProperty
	{
	public:
		IProperty(std::string &&name)
		: _name(std::move(name))
		{
			std::hash<std::string> hashFn;
			_hash = hashFn(_name);
		}

		virtual ~IProperty() {};
		IProperty &update(IProgramResources* const p)
		{
			_mutex.lock();
			_update(p);
			_mutex.unlock();
			return (*this);
		}
		IProperty &update_instancied(IProgramResources* const p)
		{
			_mutex.lock();
			_update_instancied(p);
			_mutex.unlock();
			return (*this);
		}
		virtual std::shared_ptr<IProgramResources> get_resource(std::shared_ptr<Program> const &p) = 0;
		inline std::string const &name() const { return _name; }
		inline std::size_t const &hash() const { return _hash; }

		template <typename T>
		void autoSet(const T &value)
		{
			_autoSet((void*)(&value));
		}
	private:
		virtual void _autoSet(void *dataPtr)
		{
			AGE_ASSERT(false);
		}

	protected:
		virtual void _update(IProgramResources *p) = 0;
		virtual void _update_instancied(IProgramResources*) { assert(false && "Not implemented in derived class"); };
		const std::string _name;
		std::size_t _hash;
		mutable AGE::SpinLock _mutex;
	};

	typedef IProperty Property;
}
