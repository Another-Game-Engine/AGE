#pragma once

#include <Utils/Serialization/MatrixSerialization.hpp>
#include <Utils/Serialization/VectorSerialization.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <Utils/Containers/Vector.hpp>

namespace AGE
{
	class MeshLoader;

	class BoundingInfos
	{
	public:
		BoundingInfos();
		void recompute();
		template <class Archive> void serialize(Archive &ar);
		inline const glm::vec3 &getCenter() const { return _center; }
	private:
		glm::vec3 _center;
		glm::vec3 _min;
		glm::vec3 _max;
		glm::vec3 _size;
		glm::mat4 _transform;

		//temporary to pass in private
	public:
		void initFromList(const AGE::Vector<BoundingInfos> &col);
		void addPosition(const glm::vec3 &v);

		friend class MeshLoader;
	};

	template <class Archive>
	void BoundingInfos::serialize(Archive &ar)
	{
		ar(_center
			, _min
			, _max
			, _size
			, _transform);
	}

}