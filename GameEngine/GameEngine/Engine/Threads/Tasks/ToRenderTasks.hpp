#pragma once

#include <Render/Key.hh>
#include <Render/Properties/IProperty.hh>
#include <AssetManagement/Instance/MaterialInstance.hh>
#include <AssetManagement/Instance/MeshInstance.hh>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <TMQ/message.hpp>

namespace AGE
{
	class Engine;
	struct DrawableCollection;
	class Painter;
	class Vertices;

	namespace Tasks
	{

		class Render
		{
		public:
			struct GetWindowSize : public TMQ::FutureData < glm::uvec2 >
			{
				GetWindowSize();
			};

			struct SetWindowSize
			{
				SetWindowSize(const glm::uvec2 & _size);
				glm::uvec2 size;
			};

			struct CreateRenderContext : TMQ::FutureData < bool >
			{
				CreateRenderContext(Engine *_engine);
				Engine *engine;
			};

			struct LoadMesh : TMQ::FutureData < bool >
			{

			};

			struct ReloadShaders
			{
				std::size_t notEmpty;
			};

			struct RemoveMeshProperty
			{
				Key<Painter> meshPainter;
				Key<Properties> toRemove;

				RemoveMeshProperty(Key<Painter> const &painter, Key<Properties> const &pToRemove);
			};

			struct SetMeshProperties
			{
				Key<Painter> meshPainter;
				Key<Properties> properties;
				std::shared_ptr<Properties> propertiesPtr;

				SetMeshProperties(Key<Painter> const &_meshPainter
					, Key<Properties> const &_properties
					, std::shared_ptr<Properties> const &_propertiesPtr) :
					meshPainter(_meshPainter)
					, properties(_properties)
					, propertiesPtr(_propertiesPtr)
				{ }
			};

			struct ContextGrabMouse
			{
				uint32_t grabMouse;

				ContextGrabMouse(bool _grabMouse) :
					grabMouse(_grabMouse == true ? 1 : 0)
				{ }
			};
		};
	
	}
}