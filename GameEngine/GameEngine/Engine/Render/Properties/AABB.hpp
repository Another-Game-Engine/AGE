#pragma once

# include <vector>
# include <memory>
# include <Render/Program.hh>
# include <Render/Properties/AProperty.hh>
# include <glm/glm.hpp>

namespace AGE
{
	class AABB : public AProperty
	{
	public:
		AABB(glm::vec3 const &aabb);
		AABB(AABB &&move);
		AABB(AABB const &copy) = delete;
		AABB &operator=(AABB const &other) = delete;
		AABB &operator=(glm::vec3 const &value);

	private:
		virtual void _update(IProgramResources *p) override final;

	public:
		glm::vec3 const &get();
		AABB &set(glm::vec3 const &mat);

	private:
		glm::vec3 _aabb;
	};
}
