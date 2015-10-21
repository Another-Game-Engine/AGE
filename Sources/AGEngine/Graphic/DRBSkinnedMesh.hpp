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

		inline std::size_t getSkinningIndex() const { return _skinningMatrixIndex; }
		void setSkinningMatrix(std::size_t size);
	private:
		std::size_t _skinningMatrixIndex;
		friend class GraphicElementManager;
	}; 
}