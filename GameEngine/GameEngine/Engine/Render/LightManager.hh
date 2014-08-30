#pragma once

namespace gl
{
	class LightManager
	{
	public:
		LightManager();
		~LightManager();

	private:
		LightManager(LightManager const &copy) = delete;
		LightManager &operator=(LightManager const &l) = delete;
	};
}