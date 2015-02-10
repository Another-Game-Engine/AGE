#pragma once

#include <set>
#include <Core/AScene.hh>
#include <Entities/Entity.hh>
#include <functional>

namespace AGE
{
	class EntityFilter
	{
	public:
		EntityFilter(std::weak_ptr<AScene> &&scene);
		virtual ~EntityFilter();

		template <typename T>
		void requireComponent()
		{
			_barcode.setComponent(Component<T>::getTypeId());
			_scene.lock()->filterSubscribe(Component<T>::getTypeId(), this);
		}

		template <typename T>
		void unRequireComponent()
		{
			_barcode.unsetComponent(Component<T>::getTypeId());
			_scene.lock()->filterUnsubscribe(Component<T>::getTypeId(), this);
		}

		void requireTag(TAG_ID tag);
		void unRequireTag(TAG_ID tag);

		std::set<Entity> &getCollection();

		inline void clearCollection() { _collection.clear(); }

		void virtual componentAdded(const EntityData &e, ComponentType typeId);
		void virtual componentRemoved(const EntityData &e, ComponentType typeId);
		void virtual tagAdded(const EntityData &e, TAG_ID typeId);
		void virtual tagRemoved(const EntityData &e, TAG_ID typeId);
		void virtual entityAdded(const EntityData &e);
		void virtual entityRemoved(const EntityData &e);

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
		Barcode _barcode;
		std::set<Entity> _collection;
		std::weak_ptr<AScene> _scene;

		void lock();
		void unlock();

	private:
		bool _locked;
		std::set<Entity> _trash;
		std::set<Entity> _toAdd;
		std::function<void(Entity e)> _onAdd;
		std::function<void(Entity e)> _onRemove;
	};
}