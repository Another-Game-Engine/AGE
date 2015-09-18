#pragma once

#include <Entity/Entity.hh>
#include <map>
#include <set>

namespace AGE
{
	class EntityIdRegistrationManager
	{
	public:

		struct GraphNodeUnserialize
		{
			bool haveParent;
			std::size_t parent;
			std::set<std::size_t> childs;
		};

		EntityIdRegistrationManager();
		virtual ~EntityIdRegistrationManager();
		void registrarUnserializedEntity(Entity e, std::size_t id);
		std::size_t registrarSerializedEntity(std::size_t id);
		void entityHandle(std::size_t id, Entity *e);
		void updateEntityHandles();
		void registrarGraphNode(std::size_t e, GraphNodeUnserialize g);
	private:
		void updateEntityHandle(Entity e, std::size_t id);
	private:
		std::map<std::size_t, Entity> _unser;
		std::map<std::size_t, std::size_t> _ser;
		std::multimap <std::size_t, Entity*> _toUpdate;
		std::map<Entity, GraphNodeUnserialize> _graphNode;
	};
}