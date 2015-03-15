#pragma once

# include <Render/GeometryManagement/Data/BlockMemory.hh>
# include <Render/Buffer/ABuffer.hh>
# include <Render/ProgramResources/Types/ProgramResourcesType.hh>
# include <memory>

namespace AGE
{
	class Buffer
	{
	public:
		Buffer(std::string &&name, std::unique_ptr<ABuffer> &&buffer);
		Buffer(Buffer const &copy) = delete;
		Buffer(Buffer &&move);

	public:
		size_t size() const;
		std::shared_ptr<BlockMemory> const &push_back(std::vector<uint8_t> &&data);
		Buffer &clear();
		Buffer &erase(size_t index);
		Buffer &bind();
		Buffer &unbind();
		Buffer &update();
		Buffer &require_resize();
		Buffer &require_transfer();
		std::shared_ptr<BlockMemory> const &operator[](size_t index);
		std::string const &name() const { return _name; }

	private:
		bool _request_resize;
		bool _request_transfer;
		size_t _size_alloc;
		std::vector<std::shared_ptr<BlockMemory>> _block_memories;
		std::unique_ptr<ABuffer> _buffer;
		std::string _name;
	};
}