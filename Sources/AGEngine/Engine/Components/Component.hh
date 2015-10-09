#pragma once

#include <string>
#include <Entity/Entity.hh>

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
	class ComponentRegistrationManager;

	//Declare that MACRO in each component header files
#define AGE_COMPONENT_UNIQUE_IDENTIFIER(Name) \
	public: \
	static const char *getSerializableName() { return Name; } \
	static std::size_t getSerializableId() { static const std::size_t hash = std::hash<std::string>()(std::string(Name)); return hash; } \
	

	struct ComponentBase
	{
		ComponentBase();
		virtual ~ComponentBase();
		virtual void reset(){};
		virtual void postUnserialization(){}

		void copyFrom(const ComponentBase *model);

		Entity entity;

		inline ComponentType getType() const
		{
			assert(_typeId != -1);
			return _typeId;
		}

		virtual bool doSerialize();

#ifdef EDITOR_ENABLED
		// return true if modified
		virtual bool editorUpdate();
		virtual bool isExposedInEditor();
		bool deletableInEditor = true;
		// if serialized in final export
		virtual bool serializeInExport();
#endif
	protected:
		static ComponentType _typeCounter;

		// override this method to enable copy of the component
		virtual void _copyFrom(const ComponentBase *model) = 0;
	private:
		ComponentType _typeId;

		friend class ComponentManager;
		friend class ComponentRegistrationManager;
	};


	template <typename Cpt>
	struct Component : public ComponentBase
	{
		static ComponentType getTypeId()
		{
			static ComponentType type = _typeCounter++;
			return type;
		}

		static const char *getName()
		{
			return typeid(Cpt).name();
		}
	};
}