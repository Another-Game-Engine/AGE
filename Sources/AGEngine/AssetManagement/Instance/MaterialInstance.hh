#pragma once

#include <string>
#include <vector>
#include <memory>
#include <Threads/RenderThread.hpp>

#include <glm/glm.hpp>

namespace AGE
{
	class AssetsManager;
	class Texture2D;

	struct MaterialInstance
	{
		float shininess                          = 1.0f;
		glm::vec4 diffuse                        = glm::vec4(0.5, 0.5, 0.5, 1.0f);
		glm::vec4 ambient	                     = glm::vec4(0.5, 0.5, 0.5, 1.0f);
		glm::vec4 emissive	                     = glm::vec4(0.5, 0.5, 0.5, 1.0f);
		glm::vec4 reflective                     = glm::vec4(0.5, 0.5, 0.5, 1.0f);
		glm::vec4 specular	                     = glm::vec4(0.5, 0.5, 0.5, 1.0f);
		std::shared_ptr<Texture2D> normalTex     = nullptr;
		std::shared_ptr<Texture2D> diffuseTex    = nullptr;
		std::shared_ptr<Texture2D> ambientTex    = nullptr;
		std::shared_ptr<Texture2D> emissiveTex   = nullptr;
		std::shared_ptr<Texture2D> reflectiveTex = nullptr;
		std::shared_ptr<Texture2D> specularTex   = nullptr;

		bool scaleUVs = false;
	};

	struct MaterialSetInstance
	{
	public:
		MaterialSetInstance()
		{
			_valid = false;
		}
		std::string name;
		std::string path;
		std::vector<MaterialInstance> datas;

		bool isValid()
		{
			return _valid;
		}
	private:
		std::atomic<bool> _valid;
		friend class AssetsManager;
	};

}

