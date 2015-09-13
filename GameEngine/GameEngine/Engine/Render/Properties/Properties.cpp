#include <Render/Properties/Properties.hh>
#include <Utils/Profiler.hpp>
#include <string>
#include <functional>

namespace AGE
{

	Properties::Properties(std::vector<std::shared_ptr<IProperty>> const &properties)
	{
		RWLockGuard lock(_lock, true);
		_properties = properties;
		_shaderHash = 0;
	}

	Properties::Properties(Properties &&other)
	{
		RWLockGuard lock(_lock, true);
		RWLockGuard lockO(other._lock, false);
		_properties = std::move(other._properties);
		_shaderHash = 0;
	}

	Properties::Properties(Properties const &other)
	{
		RWLockGuard lock(_lock, true);
		RWLockGuard lockO(other._lock, false);
		_properties = other._properties;
		_shaderHash = 0;
	}

	Properties::~Properties()
	{
		RWLockGuard lock(_lock, true);
		auto debug = _properties.size();
		for (size_t i = 0; i < _properties.size(); ++i)
		{
			_properties[i] = nullptr;
		}
		_properties.clear();
	}

	Key<Property> Properties::add_property(std::shared_ptr<IProperty> const &prop, bool computeHash)
	{
		SCOPE_profile_cpu_function("RenderTimer");

		RWLockGuard lock(_lock, true);
		_properties.emplace_back(prop);
		if (computeHash)
		{
			_computeHash();
		}
		return (Key<Property>::createKey(int(_properties.size()) - 1));
	}

	void Properties::merge_properties(const Properties &other)
	{
		SCOPE_profile_cpu_function("RenderTimer");

		_lock.ReadLock();
		RWLockGuard lockO(other._lock, false);
		for (auto &p : other._properties)
		{
			_lock.ReadUnlock();
			add_property(p, false);
			_lock.ReadLock();
		}
		_computeHash();
		_lock.ReadUnlock();
	}

	std::size_t Properties::getProgramId(std::size_t programId)
	{
		if (_shadersGivenId.size() <= programId)
		{
			return -1;
		}
		return _shadersGivenId[programId];
	}

	void Properties::setProgramId(std::size_t programUniqueId, std::size_t givenId)
	{
		if (_shadersGivenId.size() <= programUniqueId)
		{
			_shadersGivenId.resize(programUniqueId + 1, -1);
		}
		//AGE_ASSERT(_shadersGivenId[programUniqueId] == -1);
		_shadersGivenId[programUniqueId] = givenId;
	}

	void Properties::_computeHash()
	{
		std::string str;
		for (auto &p : _properties)
		{
			str += p->name();
		}
		std::hash<std::string> hashFn;
		_shaderHash = hashFn(str);
	}

	// @Dorian !
	// NOT WORKING -> you invalid Key doing that !
	void Properties::remove_property(Key<IProperty> const &prop)
	{
		SCOPE_profile_cpu_function("RenderTimer");

		RWLockGuard lock(_lock, true);
		if (prop.getId() != _properties.size() - 1)
		{
			std::swap(_properties[prop.getId()], _properties[_properties.size() - 1]);
		}
		_properties.resize(_properties.size() - 1);
	}

	void Properties::update_property(IProgramResources *p, std::size_t index) const
	{
		SCOPE_profile_cpu_function("RenderTimer");

		RWLockGuard lock(_lock, false);
		_properties[index]->update(p);
	}
}