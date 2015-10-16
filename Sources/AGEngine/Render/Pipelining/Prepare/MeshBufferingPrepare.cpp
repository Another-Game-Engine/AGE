#include "MeshBufferingPrepare.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <Graphic/DRBMeshData.hpp>
#include <Graphic/DRBMesh.hpp>

namespace AGE
{	
	namespace MeshBuffering
	{
		void RawType::Treat(const BFCItem &item, BFCArray<RawType> &result)
		{
			DRBMeshData * mesh = ((DRBMesh*)(item.getDrawable()))->getDatas().get();
			RawType h;
			h.vertice = ConcatenateKey(mesh->getPainterKey(), mesh->getVerticesKey());
			h.material = ((DRBMesh*)(item.getDrawable()))->material;
			h.matrix = mesh->getTransformation();
			result.push(h);
		}

		bool RawType::Compare(const RawType &a, const RawType &b)
		{
			if (a.material == b.material)
			{
				return a.vertice < b.vertice;
			}
			return a.material < b.material;
		}

		RawType RawType::Invalid()
		{
			RawType invalid;
			invalid.material = nullptr;
			invalid.vertice = -1;
			return invalid;
		}

		bool RawType::operator!=(const RawType &o) const
		{
			return (material != o.material || vertice != o.vertice);
		}

		//////////////////////////////////////////////////////////////////////////////////


	}
}
