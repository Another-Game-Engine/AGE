#include <Render/Properties/AABB.hpp>
#include <Render/ProgramResources/Types/Uniform/Vec3.hh>
#include <mutex>

namespace AGE
{

	AABB::AABB(glm::vec3 const &aabb) :
		AProperty(std::string("AABB")),
		_aabb(aabb)
	{

	}

	AABB::AABB(AABB &&move) :
		AProperty(std::move(move)),
		_aabb(move._aabb)
	{

	}

	AABB &AABB::operator=(glm::vec3 const &other)
	{
		if (_aabb != other) {
			_aabb = other;
		}
		return (*this);
	}

	void AABB::_update(IProgramResources *program)
	{
		auto resource = (Vec3*)(program);
		*resource = _aabb;
	}

	glm::vec3 const & AABB::get()
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		return (_aabb);
	}

	AABB & AABB::set(glm::vec3 const &mat)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		_aabb = mat;
		return (*this);
	}
}
