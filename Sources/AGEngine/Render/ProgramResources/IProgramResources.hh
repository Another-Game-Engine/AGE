#pragma once

#include <Utils/OpenGL.hh>
#include <vector>
#include <Utils/StringID.hpp>

namespace AGE
{
	class Program;

	class IProgramResources
	{
	public:
		IProgramResources(GLuint id, const StringID &name, GLenum type)
			: _id(id), _name(name), _type(type) {}

		virtual IProgramResources &update() = 0;
		virtual void print() const = 0;
		virtual bool safe(size_t size) const = 0;
		virtual size_t size() const = 0;

		inline const StringID &name() const { return _name; }
		GLuint id() const { return _id; }
		GLenum type() const { return _type; }

		inline void addInstanciedAlias(const StringID &alias) { _instanciedAlias.push_back(alias); }
		inline bool isInstancied() const { return (_instanciedAlias.empty() == false); }
		inline const std::vector<StringID> &getInstanciedAlias() const { return _instanciedAlias; }
	protected: 
		bool _isInstancied = false;
		std::vector<StringID> _instanciedAlias;
		GLuint _id;
		StringID _name;
		GLenum _type;
		bool _update = false;
	};

	typedef IProgramResources ProgramResource;
}