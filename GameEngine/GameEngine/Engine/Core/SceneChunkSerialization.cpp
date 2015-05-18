#include "SceneChunkSerialization.hpp"

namespace AGE
{
	SceneChunkSerialization SceneChunkSerialization::CreateForSerialization(const CptIdsRefTable &refTable, std::vector<EntitySerializationInfos> entities)
	{
		SceneChunkSerialization instance;
		instance.componentsIdReferenceTable = refTable;

		for (auto &e : entities)
		{
			auto &components = e.entity.getComponentList();
			for (auto &c : components)
			{
				if (c)
				{
#ifdef EDITOR_ENABLED
					if (WESerialization::SerializeForEditor() == false && !c->serializeInExport())
					{
						continue;
					}
#endif
					e.componentTypes.push_back(c->getType());
					e.components.push_back(c);
				}
			}
			instance.list.push_back(e);
		}

		instance.entityNbr = entities.size();
		return instance;
	}

	SceneChunkSerialization SceneChunkSerialization::CreateForLoad(AScene *scene)
	{
		SceneChunkSerialization instance;
		instance.sceneToLoad = scene;
		return instance;
	}
}