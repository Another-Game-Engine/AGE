#pragma once

#include <Systems/System.h>
#include <Components\CameraComponent.hpp>

#define MAX_SCENE_NAME_LENGTH 255

namespace AGE
{
	namespace WE
	{
		class EntityManager : public System
		{
		public:
			EntityManager(AScene *scene);
			virtual ~EntityManager();
		private:
			EntityFilter _filter;
			EntityFilter _meshRenderers;
			int pipelineIndex = 0;
			bool _graphNodeDisplay;
			bool _selectParent;
			CameraComponent *_cam = nullptr;

			std::vector<const char*> _entityNames;
			std::vector<Entity> _entities;
			Entity *_selectedEntity;
			int _selectedEntityIndex;
			char _sceneName[MAX_SCENE_NAME_LENGTH];
			char _archetypeName[MAX_SCENE_NAME_LENGTH];

			virtual void updateBegin(float time);
			virtual void updateEnd(float time);
			virtual void mainUpdate(float time);
			virtual bool initialize();
			void generateBasicEntities();
		};
	}
}