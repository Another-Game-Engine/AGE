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

			//TEMPORARY FOR TEST
			void setShader(const std::string &_shader);
			void setMesh(const std::shared_ptr<ObjFile> &_mesh);
			std::shared_ptr<ObjFile> getMesh();
			const std::string &getShader();
		protected:
			std::shared_ptr<ObjFile> mesh;
			std::string shader;
		};
	}
}