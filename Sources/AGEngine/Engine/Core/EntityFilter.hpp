#pragma once

#include <set>
#include <Entity/Entity.hh>
#include <functional>
#include <Entity/EntityTypedef.hpp>

namespace AGE
{
	class ASCene;

	class EntityFilter
	{
	public:
		EntityFilter(AScene *scene);
		virtual ~EntityFilter();

		template <typename T>
		inline void requireComponent()
		{
			requireComponent(Component<T>::getTypeId());
		}

		template <typename T>
		inline void unRequireComponent()
		{
			unRequireComponent(Component<T>::getTypeId());
		}

		void requireComponent(ComponentType id);
		void unRequireComponent(ComponentType id);

		void requireTag(TAG_ID tag);
		void unRequireTag(TAG_ID tag);

		std::set<Entity> &getCollection();
		const std::set<Entity> &getCollection() const;

		inline void clearCollection() { _collection.clear(); }

		void virtual componentAdded(const EntityData &e, ComponentType typeId);
		void virtual componentRemoved(const EntityData &e, ComponentType typeId);
		void virtual tagAdded(const EntityData &e, TAG_ID typeId);
		void virtual tagRemoved(const EntityData &e, TAG_ID typeId);
		void virtual entityAdded(const EntityData &e);
		void virtual entityRemoved(const EntityData &e);

		// use with precaution
		void manuallyRemoveEntity(const Entity &e);

		inline void setOnAdd(std::function<void(Entity e)> &onAdd) { _onAdd = onAdd; }
		inline void setOnRemove(std::function<void(Entity e)> &onRm) { _onRemove = onRm; }

		bool isLocked() const;

		struct Lock
		{
			Lock(EntityFilter &filter)
				: _filter(filter)
			{
				_filter.lock();
			}

			~Lock()
			{
				release();
			}

			void release()
			{
				_filter.unlock();
			}
		private:
			EntityFilter &_filter;
		};

	protected:
		std::set<ComponentType> _barcode;
		std::set<Entity> _collection;
		AScene *_scene;

		void lock();
		void unlock();

		bool match(const EntityData &e);

	private:
		bool _locked;
		std::set<Entity> _trash;
		std::set<Entity> _toAdd;
		std::function<void(Entity e)> _onAdd;
		std::function<void(Entity e)> _onRemove;
	};
}