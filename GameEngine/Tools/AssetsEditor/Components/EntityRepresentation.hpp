#pragma once

#include <Components/Component.hh>
#include <array>
#include <glm/fwd.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/array.hpp>
#include <WorldEditor/Managers/LayerManager.hpp>
#include <Core/AScene.hh>

#define ENTITY_NAME_LENGTH 128

namespace AGE
{
	namespace WE
	{
		struct EntityRepresentation : public ComponentBase
		{
			EntityRepresentation();

			virtual ~EntityRepresentation(void);

			void init(const char* name = "NoName", const std::string &layerName = "");

			void setLayer(const std::string &layerName)
			{
				auto layerManager = entity.getScene()->getInstance<LayerManager>();
				auto layerPtr = layerManager->getLayer(layerName);
				if (layerPtr == nullptr)
				{
					layerPtr = layerManager->getLayer(WE::ApplicationReservedLayerName);
				}
				layer = layerPtr;
				layer->addEntity(entity);
			}

			virtual void reset();

			template <typename Archive>
			void serialize(Archive &ar)
			{
				std::string sname = name;
				ar(cereal::make_nvp("name", sname));
				sname.resize(ENTITY_NAME_LENGTH - 1);
				memcpy(name, sname.data(), sname.length());
				name[ENTITY_NAME_LENGTH - 1] = 0;
			}

			virtual void postUnserialization();

			char name[ENTITY_NAME_LENGTH];
			glm::vec3 position;
			glm::vec3 rotation;
			glm::vec3 scale;
			std::shared_ptr<WE::Layer> layer;
		};
	}
}