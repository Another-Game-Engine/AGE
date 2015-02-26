#pragma once

# include <Render/ProgramResources/AProgramResources.hh>
# include <glm/glm.hpp>

namespace AGE
{
	class Program;

	class Mat4Array255 : public AProgramResources
	{
	public:
		Mat4Array255(glm::mat4 *value, GLint id, std::string &&name);
		Mat4Array255(Mat4Array255 &&move);
		Mat4Array255(Mat4Array255 const &copy);
		Mat4Array255 &operator=(Mat4Array255 const &m) = delete;
		Mat4Array255 &operator=(glm::mat4 *value);

	public:
		virtual IProgramResources &update() override final;
		virtual bool safe(size_t size) const override final;
		virtual size_t size() const override final;
		virtual void print() const override final;

	public:
		typedef glm::mat4 *type_t;

	private:
		glm::mat4 *_value;
	};
}