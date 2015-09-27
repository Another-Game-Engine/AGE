#include "ConvertorStatusManager.hpp"
#include <imgui/imgui.h>

namespace AGE
{
	namespace AE
	{
		std::size_t ConvertorStatusManager::PushTask(const std::string &message)
		{
			std::lock_guard<std::mutex> lock(_mutex);
			static std::size_t id = 0;
			_tasks.insert(std::make_pair(id, message));
			return id++;
		}

		void ConvertorStatusManager::PopTask(std::size_t id)
		{
			std::lock_guard<std::mutex> lock(_mutex);
			_tasks.erase(_tasks.find(id));
		}

		void ConvertorStatusManager::DisplayTasks()
		{
			{
				_text.clear();
				std::lock_guard<std::mutex> lock(_mutex);
				for (auto &e : _tasks)
				{
					_text += e.second;
					_text += "\n";
				}
			}
			ImGui::Text(_text.c_str());
		}
	}
}