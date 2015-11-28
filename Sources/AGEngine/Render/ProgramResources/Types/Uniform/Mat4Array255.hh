#pragma once

# include <Render/ProgramResources/IProgramResources.hh>
# include <glm/glm.hpp>

namespace AGE
{
	class Program;

	class Mat4Array255 : public IProgramResources
	{
	public:
		Mat4Array255(GLint id, std::string &&name);
		Mat4Array255(Mat4Array255 &&move);
		Mat4Array255(Mat4Array255 const &copy);
		Mat4Array255 &operator=(Mat4Array255 const &m) = delete;
		void set(glm::mat4 *value, std::size_t size);

	public:
		virtual IProgramResources &update() override final;
		virtual bool safe(size_t size) const override final;
		virtual size_t size() const override final;
		virtual void print() const override final;

	public:
		typedef glm::mat4 *type_t;

	private:
		glm::mat4 _value[255];
		std::size_t _size;
	};
}