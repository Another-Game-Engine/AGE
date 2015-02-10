#pragma once

#define EDITOR_ENABLED

#include <string>
#include <Entities/Entity.hh>

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/string.hpp>

namespace AGE
{
	class AScene;

	struct ComponentBase
	{
		ComponentBase();
		virtual ~ComponentBase();
		virtual void reset(){};

		Entity entity;

#ifdef EDITOR_ENABLED
		virtual void editorCreate(AScene *scene){}
		virtual void editorDelete(AScene *scene){}
		virtual void editorUpdate(AScene *scene){}
		bool exposedInEditor = true;
		bool deletableInEditor = true;
#endif
	protected:
		static ComponentType _typeCounter;
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