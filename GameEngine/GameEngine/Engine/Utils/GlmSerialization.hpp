#ifndef   __GLM_SERIALIZATION_HPP__
# define  __GLM_SERIALIZATION_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/common.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/array.hpp>
#include <array>

namespace cereal
{
	template<typename Archive>
	void save(Archive &ar, const glm::vec2 &v)
	{
		ar(cereal::make_nvp("x", v.x), cereal::make_nvp("y", v.y));
	}

	template<typename Archive>
	void load(Archive &ar, glm::vec2 &v)
	{
		float x, y;
		ar(x, y);
		v = glm::vec2(x, y);
	}

	template<typename Archive>
	void save(Archive &ar, const glm::vec3 &v)
	{
		ar(cereal::make_nvp("x", v.x),cereal::make_nvp("y", v.y), cereal::make_nvp("z", v.z));
	}

	template<typename Archive>
	void load(Archive &ar, glm::vec3 &v)
	{
		float x, y, z;
		ar(x, y, z);
		v = glm::vec3(x, y, z);
	}

	template<typename Archive>
	void save(Archive &ar, const glm::vec4 &v)
	{
		ar(cereal::make_nvp("x", v.x), cereal::make_nvp("y", v.y), cereal::make_nvp("z", v.z), cereal::make_nvp("w", v.w));
	}

	template<typename Archive>
	void load(Archive &ar, glm::vec4 &v)
	{
		float x, y, z, w;
		ar(x, y, z, w);
		v = glm::vec4(x, y, z, w);
	}

	template<typename Archive>
	void save(Archive &ar, const glm::mat4 &v)
	{

		double dArray[16] = { 0.0 };
		const float *pSource = (const float*)glm::value_ptr(v);
		std::cout << "float : " << sizeof(glm::value_ptr(v)) * v.length() << std::endl;
		std::cout << "double : " << sizeof(dArray) << std::endl;
		for (int i = 0; i < 16; ++i)
			dArray[i] = pSource[i];
		ar(cereal::make_nvp("mat4", dArray));
	}

	template<typename Archive>
	void load(Archive &ar, glm::mat4 &v)
	{
		double source[16];
		ar(source);
		v = glm::make_mat4(source);
	}
}



#endif    //__GLM_SERIALIZATION_HPP__