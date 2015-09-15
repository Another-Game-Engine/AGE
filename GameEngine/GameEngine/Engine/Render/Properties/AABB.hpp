#pragma once

# include <vector>
# include <memory>
# include <Render/Program.hh>
# include <Render/Properties/AProperty.hh>
# include <glm/glm.hpp>

namespace AGE
{
	class Vec3;

	class AABB : public AProperty
	{
	public:
		AABB(glm::vec3 const &aabb);
		AABB(AABB &&move);
		AABB(AABB const &copy) = delete;
		AABB &operator=(AABB const &other) = delete;
		AABB &operator=(glm::vec3 const &value);
		PROPERTY_UPDATE_FUNCTION(AABB, Vec3);
		PROPERTY_INSTANCIED_UPDATE_FUNCTION(AABB, Vec3);

	public:
		glm::vec3 const &get();
		AABB &set(glm::vec3 const &mat);

	private:
		glm::vec3 _aabb;
	};
}
