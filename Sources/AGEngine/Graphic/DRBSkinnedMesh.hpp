#pragma once

#include "DRBMesh.hpp"

namespace AGE
{
	class SkeletonProperty;
	class GraphicElementManager;

	struct DRBSkinnedMesh : public DRBMesh
	{
	public:
		DRBSkinnedMesh();
		virtual ~DRBSkinnedMesh();

		// Will returm DRBMesh
		// virtual CullableTypeID getBFCType() const;

		inline std::shared_ptr<SkeletonProperty> getSkeletonProperty() const { return _skeletonProperty; }
		void setSkinningMatrix(const glm::mat4 *skinningMatrix, std::size_t size);
	private:
		std::shared_ptr<SkeletonProperty> _skeletonProperty;
		friend class GraphicElementManager;
	}; 
}