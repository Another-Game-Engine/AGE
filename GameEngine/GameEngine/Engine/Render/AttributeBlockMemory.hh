#pragma once

# include <queue>
# include <vector>
# include <memory>
# include <utility>
# include <Render/Attribute.hh>
# include <Render/Buffer.hh>
# include <glm/glm.hpp>

template <typename type_t> class Key;

class Data;
class Buffer;

class AttributeBlockMemory
{
public:
	struct AttributeElement 
	{
		AttributeElement(std::weak_ptr<Data> const &element, glm::vec3 const &range);
		std::weak_ptr<Data> element; 
		glm::vec3 range; 
	};

public:
	AttributeBlockMemory();
	AttributeBlockMemory(Attribute type);
	AttributeBlockMemory(AttributeBlockMemory const &copy) = delete;
	AttributeBlockMemory(AttributeBlockMemory &&move);
	AttributeBlockMemory &operator=(AttributeBlockMemory const &a) = delete;
	AttributeBlockMemory &operator=(AttributeBlockMemory &&a);


public:
	AttributeBlockMemory &addElement(std::shared_ptr<Data> const &data);
	AttributeBlockMemory &updateMemory();

private:
	AttributeBlockMemory(Attribute type, size_t size, std::unique_ptr<Buffer> &&buffer);

private:
	Attribute _type;
	size_t _size;
	std::vector<AttributeElement> _elements;
	std::unique_ptr<Buffer> _buffer;
	bool _updateMajor;
	bool _updateMinor;
};