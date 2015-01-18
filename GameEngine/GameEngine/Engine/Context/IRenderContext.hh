#pragma once

#include <glm/glm.hpp>
#include <Utils/Dependency.hpp>
#include <string>

namespace AGE
{
	class IRenderContext : public Dependency < IRenderContext >
	{
	public:
		IRenderContext();
		virtual ~IRenderContext();
		bool init(int mode, unsigned int swidth, unsigned int sheight, std::string && name);
		const glm::uvec2 &getScreenSize() const;
	protected:
		glm::uvec2 _screenSize;
		std::string _windowName;
		virtual bool _init(int mode) = 0;
	};
}