#pragma once

# include <Render/ProgramResources/AProgramResources.hh>
# include <glm/glm.hpp>

namespace AGE
{
	class Program;

	class Vec3 : public AProgramResources
	{
	public:
		Vec3(glm::vec3 const &value, GLint id, std::string &&name);
		Vec3(Vec3 &&move);
		Vec3(Vec3 const &copy);
		Vec3 &operator=(Vec3 const &v) = delete;
		Vec3 &operator=(glm::vec3 const &value);

	public:
		virtual IProgramResources &update() override final;
		virtual bool safe(size_t size) const override final;
		virtual size_t size() const override final;
		virtual void print() const override final;

	public:
		typedef glm::vec3 type_t;

	private:
		glm::vec3 _value;
	};
}