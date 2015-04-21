#pragma once

#include "DepthMapManager.hpp"

namespace AGE
{
	class DepthMap;

	class DepthMapHandle
	{
	public:
		friend class DepthMapManager;
		~DepthMapHandle();
		const DepthMap *operator->();
		bool isValid() const;
		bool isWritable() const;
		std::vector<uint32_t> &getWritableBuffer();
		void setMV(const glm::mat4 &mv);
	private:
		DepthMapHandle(AGE::DepthMapManager *managerPtr, DepthMap *map, std::size_t index, DepthMapManager::Status status);
		DepthMapManager *_manager = nullptr;
		DepthMap *_map = nullptr;
		const std::size_t _index = -1;
		DepthMapManager::Status _status;
	};
}