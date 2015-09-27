#pragma once

#include <System/System.h>
#include <ComponentsCore\CameraComponent.hpp>

#define MAX_SCENE_NAME_LENGTH 255

namespace AGE
{
	struct MaterialSetInstance;
	struct MeshInstance;

	namespace WE
	{
		class EntityManager : public System<EntityManager>
		{
		public:
			EntityManager(AScene *scene);
			virtual ~EntityManager();
			void updateMenu();
		private:
			EntityFilter _filter;
			EntityFilter _meshRenderers;
			int _pipelineToSet = -1;
			bool _graphNodeDisplay;
			bool _selectParent;
			CameraComponent *_cam = nullptr;

			std::vector<const char*> _entityNames;
			std::vector<Entity> _entities;
			Entity *_selectedEntity;
			Entity *_lastFrameSelectedEntity;
			Entity _gizmoEntity;
			int _selectedEntityIndex;
			char _sceneName[MAX_SCENE_NAME_LENGTH];
			char _archetypeName[MAX_SCENE_NAME_LENGTH];
			bool _displayWindow;
			bool _reloadScene = false;
			bool _saveScene = false;
			std::shared_ptr<MaterialSetInstance> _gizmoMaterial = nullptr;
			std::shared_ptr<MeshInstance> _gizmoMesh = nullptr;

			virtual void updateBegin(float time);
			virtual void updateEnd(float time);
			virtual void mainUpdate(float time);
			virtual bool initialize();
			void generateBasicEntities();
		};
	}
}