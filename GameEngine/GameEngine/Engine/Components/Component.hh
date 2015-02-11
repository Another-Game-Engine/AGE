#pragma once

#define EDITOR_ENABLED

#include <string>
#include <Entities/Entity.hh>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/polymorphic.hpp>

#include <cereal/types/string.hpp>

namespace AGE
{
	class AScene;
	class ComponentManager;

	struct ComponentBase
	{
		ComponentBase();
		virtual ~ComponentBase();
		virtual void reset(AScene *){};

		Entity entity;

		inline ComponentType getType() const
		{
			assert(_typeId != -1);
			return _typeId;
		}

#ifdef EDITOR_ENABLED
		virtual void editorCreate(AScene *scene){}
		virtual void editorDelete(AScene *scene){}
		virtual void editorUpdate(AScene *scene){}
		bool exposedInEditor = true;
		bool deletableInEditor = true;
#endif
	protected:
		static ComponentType _typeCounter;
	private:
		ComponentType _typeId;

		friend class ComponentManager;
	};


	template <typename Cpt>
	struct Component : public ComponentBase
	{
		static ComponentType getTypeId()
		{
			static ComponentType type = _typeCounter++;
			return type;
		}
	};
}