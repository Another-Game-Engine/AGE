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
		virtual IProgramResources &update() = 0;
		virtual const StringID &name() const = 0;
		virtual GLuint id() const = 0;
		virtual GLenum type() const = 0;
		virtual void print() const = 0;
		virtual bool safe(size_t size) const = 0;
		virtual size_t size() const = 0;

		inline void addInstanciedAlias(const StringID &alias) { _instanciedAlias.push_back(alias); }
		inline bool isInstancied() const { return (_instanciedAlias.empty() == false); }
		inline const std::vector<StringID> &getInstanciedAlias() const { return _instanciedAlias; }
	protected: 
		bool _isInstancied = false;
		std::vector<StringID> _instanciedAlias;
	};

	typedef IProgramResources ProgramResource;
}