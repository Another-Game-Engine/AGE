#pragma once

#include <glm/glm.hpp>
#include <stdint.h>
#include <OpenGL/Key.hh>

#define DECL_MATERIAL(name, type, start) struct name { typedef type return_type; enum { offset = start::size + start::offset, size = sizeof(return_type) }; };

namespace gl
{
	class Texture;

	// definition of material handle by the engine
	struct START { enum { size = 0, offset = 0 }; };
	DECL_MATERIAL(COLOR_DIFFUSE, glm::vec4, START);
	DECL_MATERIAL(RATIO_DIFFUSE, float, COLOR_DIFFUSE);
	DECL_MATERIAL(TEXTURE_DIFFUSE, Key<Texture>, COLOR_DIFFUSE);
	DECL_MATERIAL(COLOR_SPECULAR, glm::vec4, TEXTURE_DIFFUSE);
	DECL_MATERIAL(RATIO_SPECULAR, float, COLOR_SPECULAR);
	DECL_MATERIAL(TEXTURE_SPECULAR, Key<Texture>, RATIO_SPECULAR);
	DECL_MATERIAL(COLOR_AMBIANT, glm::vec4, TEXTURE_SPECULAR);
	DECL_MATERIAL(RATIO_AMBIANT, float, COLOR_AMBIANT);
	DECL_MATERIAL(TEXTURE_AMBIENT, Key<Texture>, RATIO_AMBIANT);
	DECL_MATERIAL(COLOR_EMISSIVE, glm::vec4, TEXTURE_AMBIENT);
	DECL_MATERIAL(RATIO_EMISSIVE, float, COLOR_EMISSIVE);
	DECL_MATERIAL(TEXTURE_EMISSIVE, Key<Texture>, RATIO_EMISSIVE);
	DECL_MATERIAL(OPACITY, float, TEXTURE_EMISSIVE);
	DECL_MATERIAL(SHININESS, float, OPACITY);
	struct LAST { enum { size = SHININESS::offset + SHININESS::size }; };

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