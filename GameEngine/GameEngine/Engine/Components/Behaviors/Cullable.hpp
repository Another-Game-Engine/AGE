#pragma once

#include <Entities/EntityTypedef.hpp>
#include <cstddef>

//TEMP
#include <MediaFiles/ObjFile.hpp>
#include <memory>

class AScene;

namespace AGE
{
	namespace ComponentBehavior
	{
		struct Cullable
		{
		private:
			std::size_t _cullableId;
		public:
			void init(::AScene *scene, ENTITY_ID entityId, COMPONENT_ID componentTypeId);
			void reset(::AScene *scene, ENTITY_ID entityId);

			Cullable()
				:_cullableId(std::size_t(-1))
			{}

			virtual ~Cullable()
			{}

			Cullable(Cullable &&o)
			{
				mesh = std::move(o.mesh);
				_cullableId = o._cullableId;
				_scene = o._scene;
				draw = o.draw;
			}

			Cullable &operator=(Cullable &&o)
			{
				mesh = std::move(o.mesh);
				_cullableId = o._cullableId;
				_scene = o._scene;
				draw = o.draw;
				return *this;
			}


			//TEMPORARY FOR TEST
			void setMesh(const std::shared_ptr<ObjFile> &_mesh);
			std::shared_ptr<ObjFile> getMesh();

			// TEMPORARY
			bool draw = false;
		protected:
			std::shared_ptr<ObjFile> mesh;

			void sendMeshInfos();
		private:
			AScene *_scene = nullptr;
			Cullable(const Cullable &o);
			Cullable &operator=(const Cullable &o);
		};
	}
}