#pragma once

#include <Render/ProgramResources/AProgramResources.hh>
#include <glm/glm.hpp>
#include <vector>

namespace AGE
{
	class Mat4Array : public AProgramResources
	{
	public:
		Mat4Array(GLint id, size_t size, size_t stride, std::string &&name);
		Mat4Array(Mat4Array &&move);
		Mat4Array(Mat4Array const &copy);
		Mat4Array &operator=(Mat4Array const &m) = delete;
		Mat4Array &operator=(std::vector<glm::mat4> const &value);

	public:
		virtual IProgramResources &update() override final;
		virtual bool safe(size_t size) const override final;
		virtual size_t size() const override final;
		virtual void print() const override final;

	public:
		typedef glm::mat4 *type_t;

	private:
		size_t _stride;
		std::vector<glm::mat4> _value;
	};

}