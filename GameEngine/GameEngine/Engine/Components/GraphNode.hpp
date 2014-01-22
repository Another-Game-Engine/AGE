#ifndef    __GRAPH_NODE_HPP__
# define   __GRAPH_NODE_HPP__

#include "Component.hh"
#include <Core/Engine.hh>
#include <Entities/EntityData.hh>
#include <cereal/types/base_class.hpp>
#include <cereal/types/set.hpp>

namespace Component
{
	struct GraphNode : public ComponentBase<GraphNode>
	{
		typedef std::list<Entity> t_EntityList;

		GraphNode()
			: ComponentBase<GraphNode>()
		{
		}

		virtual ~GraphNode()
		{
		}

		void init()
		{
			_parent = Entity(std::numeric_limits<unsigned int>::max(), nullptr);

			// signal it's a root node
			auto key = PubSubKey("graphNodeSetAsRoot");
			_entity->broadCast(key, _entity);
		}

		virtual void reset()
		{
			_childs.clear();
			auto key = PubSubKey("graphNodeNotARoot");
			_entity->broadCast(key, _entity);
		}

		const Entity	    	&getParent() const
		{
			return _parent;
		}

		void 					removeSon(Entity &son, bool notify = true)
		{
			_childs.erase(son);
			if (notify && son->hasComponent<Component::GraphNode>())
			{
				son->getComponent<Component::GraphNode>()->removeParent(false);
			}
		}

		void 					setParent(Entity &father, bool notify = true)
		{
			if (_parent.get() && _parent->hasComponent<Component::GraphNode>())
			{
				_parent->getComponent<Component::GraphNode>()->removeSon(_entity, false);
			}

			if (notify && father->hasComponent<Component::GraphNode>())
				father->getComponent<Component::GraphNode>()->addSon(_entity, false);
			if (!father.get()) // if parent is null -> it's a root node
			{
				auto key = PubSubKey("graphNodeSetAsRoot");
				_entity->broadCast(key, _entity);
			}
			else if (!_parent.get()) // if it was a root node
			{
				auto key = PubSubKey("graphNodeNotARoot");
				_entity->broadCast(key, _entity);
			}
			_parent = father;
		}

		void 					addSon(Entity &son, bool notify = true)
		{
			_childs.insert(son);
			if (notify && son->hasComponent<Component::GraphNode>())
			{
				son->getComponent<Component::GraphNode>()->setParent(_entity, false);
			}
		}

		void                    removeParent(bool notify = true)
		{
			if (notify && _parent.get() && _parent->hasComponent<Component::GraphNode>())
			{
				_parent->getComponent<Component::GraphNode>()->removeSon(_entity);
			}
			auto key = PubSubKey("graphNodeSetAsRoot");
			_entity->broadCast(key, _entity);
			_parent = Entity(std::numeric_limits<unsigned int>::max(), nullptr);
		}

		std::set<Entity>::iterator getSonsBegin()
		{
			return std::begin(_childs);
		}
		
		std::set<Entity>::iterator getSonsEnd()
		{
			return std::end(_childs);
		}

		//////
		////
		// Serialization

		template <typename Archive>
		Base *unserialize(Archive &ar, Entity e)
		{
			auto res = new GraphNode();
			res->setEntity(e);
			ar(*res);
			return res;
		}

		template <typename Archive>
		void save(Archive &ar) const
		{
			std::set<std::size_t> childIds;
			for (auto e : _childs)
			{
				childIds.insert(_entity.get()->getScene()->registrarSerializedEntity(e.getId()));
			}
			ar(CEREAL_NVP(childIds));
			ar(cereal::make_nvp("haveParent", _parent.get() != nullptr));
			ar(cereal::make_nvp("parentID", _parent.getId()));
		}

		template <typename Archive>
		void load(Archive &ar)
		{
			std::set<std::size_t> childIds;
			ar(childIds);
			for (auto e : childIds)
				_childs.insert(Entity(e));
			for (auto it = std::begin(_childs); it != std::end(_childs); ++it)
			{ 
				Entity *e = const_cast<Entity *>(&(*it));
				_entity->getScene()->entityHandle(it->getId(), e);
			}
			bool haveParent = false;
			ar(haveParent);
			std::size_t parentId;
			ar(parentId);
			if (haveParent)
			{
				_entity->getScene()->entityHandle(parentId, &_parent);
			}
			else
			{
				auto key = PubSubKey("graphNodeSetAsRoot");
				_entity->broadCast(key, _entity);
			}
		}

		// !Serialization
		////
		//////

	private:
		Entity _parent;
		std::set<Entity> _childs;
	};
}

#endif     //__GRAPH_NODE_HPP__