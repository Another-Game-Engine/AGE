#ifndef   __MATERIAL_FILE_HPP__
# define  __MATERIAL_FILE_HPP__

#include <MediaFiles/MediaFile.hpp>
#include <vector>
#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/common.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/complex.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <Utils/GlmSerialization.hpp>
#include <OpenGL/UniformBuffer.hh>
#include <tiny_obj_loader.h>
#include <MediaFiles/TextureFile.hpp>

struct MaterialFile : public MediaFile<MaterialFile>
{
	MaterialFile();

	virtual ~MaterialFile();

	MaterialFile(const MaterialFile &o);

	MaterialFile &operator=(const MaterialFile &o);

	struct Material
	{
		std::string name;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		glm::vec3 transmittance;
		glm::vec3 emission;
		float shininess;
		std::shared_ptr<TextureFile> ambientTex;
		std::shared_ptr<TextureFile> diffuseTex;
		std::shared_ptr<TextureFile> specularTex;
		std::shared_ptr<TextureFile> normalTex;
		std::map<std::string, glm::vec2> paramVec2;
		std::map<std::string, glm::vec3> paramVec3;
		std::map<std::string, glm::vec4> paramVec4;
		std::map<std::string, glm::mat2> paramMat2;
		std::map<std::string, glm::mat3> paramMat3;
		std::map<std::string, glm::mat4> paramMat4;
		std::map<std::string, int> paramInt;
		std::map<std::string, float> paramFloat;
		MaterialFile *file;
	private:
		std::weak_ptr<DependenciesInjector> _dpyManager;
		friend struct MaterialFile;
	public:

		Material();

		void setUniforms(std::shared_ptr<OpenGLTools::UniformBuffer> buffer);

		Material &operator=(const Material &o);

		Material(const Material &o);

		void save(cereal::PortableBinaryOutputArchive &ar) const;
		void load(cereal::PortableBinaryInputArchive &ar);

	};

	std::vector<Material> materials;

	//AMediaFile *unserialize(cereal::PortableBinaryInputArchive &ar);

	void save(cereal::PortableBinaryOutputArchive &ar) const;

	void load(cereal::PortableBinaryInputArchive &ar);

};


#endif    //__MATERIAL_FILE_HPP__