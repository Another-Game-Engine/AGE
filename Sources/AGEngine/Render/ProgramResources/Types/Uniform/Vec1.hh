#pragma once

#include <Render/ProgramResources/IProgramResources.hh>

namespace AGE
{
	class Vec1 : public IProgramResources
	{
	public:
		Vec1(float v, GLint id, std::string &&name);
		Vec1(Vec1 &&move);
		Vec1(Vec1 const &copy);
		Vec1 &operator=(Vec1 const &v) = delete;
		Vec1 &operator=(float v);

	public:
		virtual IProgramResources &update() override final;
		virtual bool safe(size_t size) const override final;
		virtual size_t size() const override final;
		virtual void print() const override final;

	public:
		typedef float type_t;

	private:
		float _value;
	};
}