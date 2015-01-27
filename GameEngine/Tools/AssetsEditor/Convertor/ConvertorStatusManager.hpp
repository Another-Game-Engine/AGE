#pragma once

#include <Utils/Singleton.hh>
#include <unordered_map>
#include <mutex>

namespace AGE
{
	namespace AE
	{
		class ConvertorStatusManager
		{
		public:
			std::size_t PushTask(const std::string &message);
			void PopTask(std::size_t id);
			void DisplayTasks();
		private:
			ConvertorStatusManager(){}
			~ConvertorStatusManager(){}
			std::unordered_map<std::size_t, std::string> _tasks;
			std::mutex _mutex;
			std::string _text;
			friend class Singleton < ConvertorStatusManager >;
		};
	}
}