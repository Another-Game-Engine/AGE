//#include <OpenGL/ShaderResource.hh>
//#include <stdint.h>
//#include <OpenGL/Shader.hh>
//#include <glm/glm.hpp>
//#include <OpenGL/MemoryGPU.hh>
//#include <OpenGL/UniformBlock.hh>
//
//namespace gl
//{
//
//	ShaderResource::ShaderResource()
//		: _setting(NONE),
//		_location(0),
//		_data(NULL),
//		_sizeData(0)
//	{
//
//	}
//
//	ShaderResource::ShaderResource(GLuint location)
//		: ShaderResource()
//	{
//		_location = location;
//	}
//
//	ShaderResource::ShaderResource(ShaderResource const &ressource)
//		: ShaderResource()
//	{
//		_setting = ressource._setting;
//		_location = ressource._location;
//		_sizeData = ressource._sizeData;
//		_data = new uint8_t[_sizeData];
//		memcpy(_data, ressource._data, _sizeData);
//	}
//
//	ShaderResource &ShaderResource::operator=(ShaderResource const &u)
//	{
//		if (this != &u)
//		{
//			_location = u._location;
//			if (u._sizeData != _sizeData)
//			{
//				_sizeData = u._sizeData;
//				if (_data)
//					delete _data;
//				_data = new uint8_t[_sizeData];
//			}
//			memcpy(_data, u._data, _sizeData);
//		}
//		return (*this);
//	}
//
//	ShaderResource::~ShaderResource()
//	{
//		if (_data)
//			delete _data;
//	}
//
//}