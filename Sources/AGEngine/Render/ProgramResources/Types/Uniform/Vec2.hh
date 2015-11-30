#pragma once

#include <Render/ProgramResources/IProgramResources.hh>
#include <glm/glm.hpp>

namespace AGE
{
	class Program;

	class Vec2 : public IProgramResources
	{
	public:
		Vec2(glm::vec2 const &value, GLint id, std::string &&name);
		Vec2(Vec2 &&move);
		Vec2(Vec2 const &copy);
		Vec2 &operator=(Vec2 const &v) = delete;
		Vec2 &operator=(glm::vec2 const &value);

	public:
		virtual IProgramResources &update() override final;
		virtual bool safe(size_t size) const override final;
		virtual size_t size() const override final;
		virtual void print() const override final;

	public:
		typedef glm::vec2 type_t;

	private:
		glm::vec2 _value;
	};
}