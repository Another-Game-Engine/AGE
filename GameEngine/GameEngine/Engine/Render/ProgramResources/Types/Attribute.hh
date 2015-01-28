#pragma once

# include <Render/ProgramResources/AProgramResources.hh>
# include <Render/ProgramResources/Types/ProgramResourcesType.hh>

# include <memory>

namespace AGE
{
	class Attribute : public AProgramResources
	{
	public:
		Attribute(GLint index, std::string &&name, GlType const &type);
		Attribute(Attribute &&move);
		Attribute(Attribute const &copy);
		Attribute &operator=(Attribute const &a) = delete;
		template <typename type_t> Attribute &operator+=(std::vector<type_t> const &data);
		bool operator==(GLenum p) const;
		bool operator!=(GLenum p) const;

	public:
		virtual IProgramResources &update() override final;
		virtual bool safe(size_t size) const override final;
		virtual size_t size() const override final;
		virtual void print() const override final;

	public:
		typedef Attribute * type_t;

	private:
		GlType _available_type;
	};
}