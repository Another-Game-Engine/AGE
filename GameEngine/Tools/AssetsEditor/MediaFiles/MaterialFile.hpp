#pragma once

#include "MediaFile.hpp"
#include <map>

class MaterialFile : public MyMediaFile
{
public:
	MaterialFile(std::string &&path, std::string &&name, std::string &&ext)
		: MyMediaFile(std::move(path), std::move(name), std::move(ext))
	{}

	virtual ~MaterialFile()
	{}

    std::string name;
    float ambient[3];
    float diffuse[3];
    float specular[3];
    float transmittance[3];
    float emission[3];
    float shininess;
    float ior;
    std::string ambient_texname;
    std::string diffuse_texname;
    std::string specular_texname;
    std::string normal_texname;
    std::map<std::string, std::string> unknown_parameter;	
};