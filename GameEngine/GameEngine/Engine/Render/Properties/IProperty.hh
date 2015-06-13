#pragma once

#include <string>
#include <memory>
#include <vector>
#include <Render/ProgramResources/IProgramResources.hh>
#include <Utils/SpinLock.hpp>
#include <mutex>

namespace AGE
{
	class Program;

	class IProperty
	{
	public:
		virtual ~IProperty() {};
		IProperty &update(std::shared_ptr<Program> const &p)
		{
			_mutex.lock();
			_update(p);
			_mutex.unlock();
			return (*this);
		}
		virtual std::shared_ptr<IProgramResources> get_resource(std::shared_ptr<Program> const &p) = 0;
		virtual std::string const &name() const = 0;
	protected:
		virtual void _update(std::shared_ptr<Program> const &p) = 0;
		mutable AGE::SpinLock _mutex;
	};

	typedef IProperty Property;
}
