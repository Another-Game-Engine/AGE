#pragma once

# include <memory>
# include <Render/ProgramResources/AProgramResources.hh>
# include <vector>
# include <tuple>
# include <glm/glm.hpp>

namespace AGE
{
	class IInterfaceBlock;

	class BlockResources : public AProgramResources
	{
	public:
		BlockResources(GLint id, std::string &&name, GLenum type, glm::vec3 const &info);
		BlockResources(BlockResources &&move);
		template <typename type_t> BlockResources &operator=(type_t value);
		BlockResources &operator=(BlockResources const &b) = delete;

	public:
		size_t offset() const;
		size_t size_array() const;
		size_t stride() const;
		std::vector<uint8_t> const &data() const;
		BlockResources &assignation(IInterfaceBlock * interfaceBlock);

	public:
		virtual IProgramResources &update() override final;
		virtual bool safe(size_t size) const override final;
		virtual size_t size() const override final;
		virtual void print() const override final;

	private:
		IInterfaceBlock * _parent;
		size_t _offset;
		size_t _size_array;
		size_t _stride;
		size_t _size;
		std::vector<uint8_t> _data;
	};

	template <typename type_t>
	BlockResources & BlockResources::operator=(type_t value)
	{
		if (!safe(sizeof(type_t))) {
			return (*this);
		}
		std::memcpy(_data.data(), (void *)&value, sizeof(type_t));
		_update = false;
		if (_parent) {
			_parent->require_update();
		}
		return (*this);
	}
}
