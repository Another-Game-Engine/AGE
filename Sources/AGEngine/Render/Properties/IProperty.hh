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

#define PROPERTY_UPDATE_FUNCTION(PROPERTY_TYPE, RESOURCE_TYPE)\
	static void update(RESOURCE_TYPE*, PROPERTY_TYPE*);\
	static inline void _updateFunctionLock(IProgramResources *r, IProperty*p){\
	    PROPERTY_TYPE::update((RESOURCE_TYPE*)(r), (PROPERTY_TYPE*)(p));\
	}\
    inline virtual void(*getUpdateFunction())(IProgramResources *, IProperty*) {\
	    return PROPERTY_TYPE::_updateFunctionLock;\
	}\

#define PROPERTY_INSTANCIED_UPDATE_FUNCTION(PROPERTY_TYPE, RESOURCE_TYPE)\
	static void instanciedUpdate(RESOURCE_TYPE*, PROPERTY_TYPE*);\
	static inline void _instanciedUpdateFunctionLock(IProgramResources *r, IProperty*p){\
	   PROPERTY_TYPE::instanciedUpdate((RESOURCE_TYPE*)(r), (PROPERTY_TYPE*)(p));\
			}\
	inline virtual void(*getInstanciedUpdateFunction())(IProgramResources *, IProperty*) {\
	    return PROPERTY_TYPE::_instanciedUpdateFunctionLock;\
			}\


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
		virtual std::shared_ptr<IProgramResources> get_resource(std::shared_ptr<Program> const &p) = 0;
		inline std::string const &name() const { return _name; }
		inline std::size_t const &hash() const { return _hash; }
		virtual void(*getUpdateFunction())(IProgramResources *, IProperty*) = 0;
		virtual void(*getInstanciedUpdateFunction())(IProgramResources *, IProperty*) = 0;

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
		const std::string _name;
		std::size_t _hash;
		//dirty:
	};

	typedef IProperty Property;
}
