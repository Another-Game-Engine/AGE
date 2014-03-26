#pragma once

#include <Components/Component.hh>

struct Room;

namespace Component
{
	struct HotZone : public Component::ComponentBase<HotZone>
	{
		HotZone()
		{}

		virtual ~HotZone(void)
		{}

		void init(const std::string &_uniqueName, const std::string &_sharedName, std::shared_ptr<Room> _room)
		{
			uniqueName = _uniqueName;
			sharedName = _sharedName;
			room = _room;
		}

		virtual void reset()
		{
			uniqueName = "";
			sharedName = "";
			room = nullptr;
		}

		std::string uniqueName;
		std::string sharedName;
		std::shared_ptr<Room> room;

		//////
		////
		// Serialization

		template <typename Archive>
		Base *unserialize(Archive &ar, Entity e)
		{
			auto res = new HotZone();
			res->setEntity(e);
			ar(*res);
			return res;
		}

		template <typename Archive>
		void serialize(Archive &ar)
		{
			auto TODO = 0;
			ar(CEREAL_NVP(TODO));
		}

		// !Serialization
		////
		//////
	private:
		HotZone(HotZone const &);
		HotZone &operator=(HotZone const &);
	};
}