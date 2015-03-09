#pragma once

#include <glm/glm.hpp>
#include <Utils/Dependency.hpp>
#include <Core/AgeInputs.hh>
#include <string>
#include <map>

namespace AGE
{
	class IRenderContext : public Dependency < IRenderContext >
	{
	public:
		IRenderContext();
		virtual ~IRenderContext();

		bool init(int mode, unsigned int swidth, unsigned int sheight, std::string && name);
		glm::uvec2 getScreenSize() const;

		virtual void swapContext() = 0;
		virtual void refreshInputs() = 0;
		virtual void setScreenSize(const glm::uvec2 &screenSize) = 0;

	protected:
		glm::uvec2 _screenSize;
		std::string _windowName;
		static std::map<int, AgeKeys> _toAgeKey;
		static std::map<int, AgeInputs> _toAgeInput;

		AgeKeys findAgeKey(int contextKey) const;
		AgeInputs findAgeInput(int contextInput) const;

		virtual bool _init(int mode) = 0;
	};
}