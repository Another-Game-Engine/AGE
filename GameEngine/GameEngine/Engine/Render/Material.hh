#pragma once

#include <glm/glm.hpp>
#include <stdint.h>
#include <Render/Key.hh>

#define DECL_MATERIAL(name, type, start, v) struct name { typedef type return_type; enum { offset = start::size + start::offset, size = sizeof(return_type), value = v }; };

namespace gl
{
	class Texture;
	class RenderPass;

	enum TypeMaterial
	{
		COLOR_DIFFUSE = 0,
		RATIO_DIFFUSE,
		TEXTURE_DIFFUSE,
		COLOR_SPECULAR,
		RATIO_SPECULAR,
		TEXTURE_SPECULAR,
		COLOR_AMBIANT,
		RATIO_AMBIANT,
		TEXTURE_AMBIENT,
		COLOR_EMISSIVE,
		RATIO_EMISSIVE,
		TEXTURE_EMISSIVE,
		OPACITY,
		SHININESS
	};

	// definition of material handle by the engine
	struct Start { enum { size = 0, offset = 0 }; };
	DECL_MATERIAL(Color_diffuse, glm::vec4, Start, COLOR_DIFFUSE);
	DECL_MATERIAL(Ratio_diffuse, float, Color_diffuse, RATIO_DIFFUSE);
	DECL_MATERIAL(Texture_diffuse, unsigned int, Ratio_diffuse, TEXTURE_DIFFUSE);
	DECL_MATERIAL(Color_specular, glm::vec4, Texture_diffuse, COLOR_SPECULAR);
	DECL_MATERIAL(Ratio_specular, float, Color_specular, RATIO_SPECULAR);
	DECL_MATERIAL(Texture_specular, unsigned int, Ratio_specular, TEXTURE_SPECULAR);
	DECL_MATERIAL(Color_ambiant, glm::vec4, Texture_specular, COLOR_AMBIANT);
	DECL_MATERIAL(Ratio_ambiant, float, Color_ambiant, RATIO_AMBIANT);
	DECL_MATERIAL(Texture_ambiant, unsigned int, Ratio_ambiant, TEXTURE_AMBIENT);
	DECL_MATERIAL(Color_emissive, glm::vec4, Texture_ambiant, COLOR_EMISSIVE);
	DECL_MATERIAL(Ratio_emissive, float, Color_emissive, RATIO_EMISSIVE);
	DECL_MATERIAL(Texture_emissive, unsigned int, Ratio_emissive, TEXTURE_EMISSIVE);
	DECL_MATERIAL(Opacity, float, Texture_emissive, OPACITY);
	DECL_MATERIAL(Shininess, float, Opacity, SHININESS);
	struct Last { enum { size = Shininess::offset + Shininess::size }; };

	//!\file Material.hh
	//!\author Dorian Pinaud
	//!\version v2.0
	//!\class Material
	//!\brief contain Material information
	class Material
	{
	public:
		Material();
		~Material();
		Material(Material const &copy);
		Material &operator=(Material const &m);

		template <typename TYPE> typename TYPE::return_type get() const;
		template <typename TYPE> Material &set(typename TYPE::return_type const &value);
		uint8_t const *getData(size_t offset) const;

	private:
		uint8_t *_data;
	};

	template <typename TYPE>
	typename TYPE::return_type Material::get() const
	{
		TYPE::return_type *data = (TYPE::return_type *)&_data[TYPE::offset];
		return (*data);
	}

	template <typename TYPE>
	Material &Material::set(typename TYPE::return_type const &value)
	{
		TYPE::return_type *data = (TYPE::return_type *)&_data[TYPE::offset];
		memcpy(data, &value, sizeof(TYPE::return_type));
		return (*this);
	}
}