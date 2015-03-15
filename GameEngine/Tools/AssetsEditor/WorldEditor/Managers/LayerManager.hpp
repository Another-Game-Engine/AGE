#pragma once

#include <Utils/Dependency.hpp>
#include <map>
#include <unordered_set>
#include <string>

namespace AGE
{
	namespace WE
	{
		static const std::string ApplicationReservedLayerName = "APPLICATION_RESERVED_LAYER";
		static const std::size_t ApplicationReservedLayerId = 0;

		class LayerManager;

		class Layer
		{
		public:
			const std::string &getName() const
			{
				return _name;
			}

			std::size_t getId() const
			{
				return _id;
			}

			bool isApplicationReserved() const
			{
				return _id == ApplicationReservedLayerId;
			}
		private:
			Layer() = delete;
			Layer(const std::string &name, std::size_t id)
				: _name(name)
				, _id(id)
			{

			}
			friend class LayerManager;
			std::string _name;
			const std::size_t _id;
			std::unordered_set<Entity> _entities;
		};

		class LayerManager : public Dependency<LayerManager>
		{
		public:
			LayerManager()
			{
				auto applicationReserved = std::shared_ptr<Layer>(new Layer(ApplicationReservedLayerName, ApplicationReservedLayerId));
				// we don't add it to the names list
				++_layerIdCounter;
				_layers.insert(std::make_pair(ApplicationReservedLayerName, applicationReserved));
				
				// we create the default one
				auto defaultLayer = createLayer();
				_selectedId = defaultLayer->getId();
			}

			std::shared_ptr<Layer> createLayer()
			{
				auto name = "Layer_" + std::to_string(_layerIdCounter);
				auto layer = std::shared_ptr<Layer>(new Layer(name, _layerIdCounter));
				_layers.insert(std::make_pair(name, layer));
				_layerIdCounter++;
				return layer;
			}

			const std::vector<std::string> &getLayerList() const
			{
				return _names;
			}



		private:
			std::map<std::string, std::shared_ptr<Layer>> _layers;
			std::vector<std::string> _names;
			std::size_t _selectedId;
			std::size_t _layerIdCounter = ApplicationReservedLayerId;
		};
	}
}