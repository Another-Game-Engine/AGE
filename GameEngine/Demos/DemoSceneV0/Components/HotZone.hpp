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

		HotZone(HotZone const &o)
		{
			uniqueName = o.uniqueName;
			sharedName = o.sharedName;
			room = o.room;
		}

		HotZone &operator=(HotZone const &o)
		{
			uniqueName = o.uniqueName;
			sharedName = o.sharedName;
			room = o.room;
			return *this;
		}

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
		void serialize(Archive &ar)
		{
			auto TODO = 0;
			ar(CEREAL_NVP(TODO));
		}

		// !Serialization
		////
		//////
	private:
	};
}