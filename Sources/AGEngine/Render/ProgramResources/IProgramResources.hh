#pragma once

#include <Utils/OpenGL.hh>
#include <string>
#include <vector>

namespace AGE
{
	class Program;

	class IProgramResources
	{
	public:
		virtual IProgramResources &update() = 0;
		virtual std::string const &name() const = 0;
		virtual GLuint id() const = 0;
		virtual GLenum type() const = 0;
		virtual void print() const = 0;
		virtual bool safe(size_t size) const = 0;
		virtual size_t size() const = 0;

		inline void addInstanciedAlias(const std::string &alias) { _instanciedAlias.push_back(alias); }
		inline bool isInstancied() const { return (_instanciedAlias.empty() == false); }
		inline const std::vector<std::string> &getInstanciedAlias() const { return _instanciedAlias; }
	protected: 
		bool _isInstancied = false;
		std::vector<std::string> _instanciedAlias;
	};

	typedef IProgramResources ProgramResource;
}