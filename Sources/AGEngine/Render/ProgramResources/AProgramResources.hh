#pragma once

# include <Render/ProgramResources/IProgramResources.hh>
#include <Utils/StringID.hpp>

namespace AGE
{
	class Program;

	class AProgramResources : public IProgramResources
	{
	protected:
		AProgramResources(GLuint id, const StringID &name, GLenum type);
		AProgramResources(AProgramResources &&move);
		AProgramResources(AProgramResources const &copy);
		AProgramResources &operator=(AProgramResources const &a) = delete;

	public:
		virtual StringID const &name() const override final;
		virtual GLuint id() const override final;
		virtual GLenum type() const override final;
	protected:
		StringID _name;
		GLuint _id;
		GLenum _type;
		bool _update;
	};
}