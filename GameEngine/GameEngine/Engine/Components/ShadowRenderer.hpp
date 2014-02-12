#ifndef SHADOWRENDERER_HPP_
# define SHADOWRENDERER_HPP_

# include <memory>
# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp>
# include "MediaFiles/ObjFile.hpp"
# include "Components/Component.hh"
# include "Core/Renderer.hh"

namespace Component
{
	class ShadowRenderer : public Component::ComponentBase<ShadowRenderer>
	{
	public:
		ShadowRenderer();
		~ShadowRenderer();
		void init(std::shared_ptr<AMediaFile> const &media);
		void init(std::shared_ptr<ObjFile> const &obj);
		void setShader(std::string const &s);
		virtual void reset();
		void render(glm::mat4 const &VPLight);
		std::shared_ptr<ObjFile> const &getMesh() const;

		template <typename Archive>
		Base *unserialize(Archive &ar, Entity e)
		{
			auto res = new MeshRenderer();
			res->setEntity(e);
			ar(*res);
			return res;
		}

		template <typename Archive>
		void save(Archive &ar) const
		{
			ar(CEREAL_NVP(_shader));
			std::string meshName = _mesh->path.getFullName();
			ar(cereal::make_nvp("meshName", meshName));
		}

		template <typename Archive>
		void load(Archive &ar)
		{
			ar(_shader);
			std::string meshName;
			ar(meshName);
			_mesh = std::static_pointer_cast<ObjFile>(AMediaFile::loadFromFile<cereal::BinaryInputArchive>(File(meshName)));
		}

	private:
		std::shared_ptr<ObjFile> _mesh;
		std::string _shader;
	};
}

#endif /*!SHADOWRENDERER_HPP_*/