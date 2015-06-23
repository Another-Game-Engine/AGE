#include <Engine/AssetManagement/OpenGLDDSLoader.hh>
#include <glm/gtx/extented_min_max.hpp>
#include <fstream>

#include <windows.h>

namespace AGE
{
	static const GLenum textureFaces[6] =
	{
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	std::shared_ptr<ATexture> OpenGLDDSLoader::loadDDSFile(OldFile const &file)
	{
		std::ifstream ifs(file.getFullName(), std::ios::binary);
		char *fileBuffer;
		char *currentPtr;
		uint32_t magic;
		DirectX::DDS_HEADER *header;

		if (!ifs)
		{
			assert(!"The file does not exist");
			return nullptr;
		}
		// Get the file size and read it in one syscall
		ifs.seekg(0, std::ios::end);
		std::streamsize fileSize = ifs.tellg();
		ifs.seekg(0, std::ios::beg);

		fileBuffer = new char[fileSize];
		ifs.read(fileBuffer, fileSize);
		// currentPtr is the current reading pointer
		currentPtr = fileBuffer;

		if (!ifs)
		{
			// The read failed
			assert(!"The file has not been found");
			return (nullptr);
		}
		// We check the magic number
		magic = *(uint32_t*)currentPtr;
		currentPtr += sizeof(uint32_t);
		if (magic != DirectX::DDS_MAGIC)
		{
			// Wrong magic number
			delete fileBuffer;
			assert(!"Texture is not a DDS");
			return (nullptr);
		}
		// We get the header
		header = (DirectX::DDS_HEADER*)currentPtr;
		currentPtr += sizeof(DirectX::DDS_HEADER);
		size_t totalSize = 0;

		// We get the texture infos
		bool compressed;
		size_t blockOrPixelSize;
		GLenum format;
		GLenum sizedFormat;
		GLenum uncompressedType;

		// We get the texture infos (is it compressed, what kind of compression, color components...)
		if (getTexturePixelInfos(header, compressed, blockOrPixelSize, format, sizedFormat, uncompressedType) == false)
			return (nullptr);

		// We check the texture type
		bool isCubeMap = (header->dwCaps2 & DDS_CUBEMAP_ALLFACES) == DDS_CUBEMAP_ALLFACES;
		// TODO: We can add a volume texture type here
		bool hasMipmaps = (header->dwFlags & DDS_HEADER_FLAGS_MIPMAP) == DDS_HEADER_FLAGS_MIPMAP;
		size_t textureMipmaps = hasMipmaps ? header->dwMipMapCount : 1;
		size_t textureDepth;
		std::shared_ptr<ATexture> texture = nullptr;
		
		if (isCubeMap)
		{
			std::shared_ptr<TextureCubeMap> tmp = std::make_shared<TextureCubeMap>();

			tmp->init(header->dwWidth, header->dwHeight, sizedFormat, true);
			texture = tmp;
			textureDepth = 6;
		}
		else
		{
			std::shared_ptr<Texture2D> tmp = std::make_shared<Texture2D>();

			tmp->init(header->dwWidth, header->dwHeight, sizedFormat, true);
			texture = tmp;
			textureDepth = 1;
		}

		// Here we read the texture content
		size_t mipmapSize;

		// for each depth
		for (size_t curDepth = 0; curDepth < textureDepth; ++curDepth)
		{
			size_t width = header->dwWidth;
			size_t height = header->dwHeight;
			// for each mipmap
			for (size_t curMipmap = 0; curMipmap < textureMipmaps; ++curMipmap)
			{
				if (compressed)
					mipmapSize = glm::max(1ULL, ((width + 3) / 4)) * glm::max(1ULL, ((height + 3) / 4)) * blockOrPixelSize;
				else
					mipmapSize = glm::max(1ULL, width) * glm::max(1ULL, height) * blockOrPixelSize;

				if (isCubeMap)
				{
					std::shared_ptr<TextureCubeMap> tmp = std::static_pointer_cast<TextureCubeMap>(texture);

					if (compressed)
						tmp->setCompressed(textureFaces[curDepth], currentPtr, curMipmap, width, height, format, mipmapSize);
					else
						tmp->set(textureFaces[curDepth], currentPtr, curMipmap, width, height, format, uncompressedType);
				}
				else
				{
					std::shared_ptr<Texture2D> tmp = std::static_pointer_cast<Texture2D>(texture);

					if (compressed)
						tmp->setCompressed(currentPtr, curMipmap, width, height, format, mipmapSize);
					else
						tmp->set(currentPtr, curMipmap, width, height, format, uncompressedType);
				}
				currentPtr += mipmapSize;
				width >>= 1;
				height >>= 1;
			}
		}

		// If the texture has no mipmaps, we generate them
		if (hasMipmaps == false)
			texture->generateMipmaps();
		// We set the basic texture parameters
		texture->parameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
		texture->parameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
		texture->parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		texture->parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// We delete the buffer and return, texture loaded successfuly
		delete fileBuffer;
		texture->unbind();
		return (texture);
	}

	bool OpenGLDDSLoader::getTexturePixelInfos(DirectX::DDS_HEADER const *header, bool &compressed,
		size_t &blockOrPixelSize, GLenum &format, GLenum &sizedFormat, GLenum &uncompressedType)
	{
		if (memcmp(&header->ddspf, &DirectX::DDSPF_DXT5, sizeof(DirectX::DDS_PIXELFORMAT)) == 0) // Compressed texture DXT5
		{
			compressed = true;
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			sizedFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			blockOrPixelSize = 16;
		}
		else if (memcmp(&header->ddspf, &DirectX::DDSPF_DXT3, sizeof(DirectX::DDS_PIXELFORMAT)) == 0) // Compressed texture DXT3
		{
			compressed = true;
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			sizedFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			blockOrPixelSize = 16;
		}
		else if (memcmp(&header->ddspf, &DirectX::DDSPF_DXT1, sizeof(DirectX::DDS_PIXELFORMAT)) == 0) // Compressed texture DXT1
		{
			compressed = true;
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			sizedFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			blockOrPixelSize = 8;
		}
		else if (memcmp(&header->ddspf, &DirectX::DDSPF_A8R8G8B8, sizeof(DirectX::DDS_PIXELFORMAT)) == 0 ||
			memcmp(&header->ddspf, &DirectX::DDSPF_X8R8G8B8, sizeof(DirectX::DDS_PIXELFORMAT)) == 0) // Uncompressed textures
		{
			compressed = false;
			format = GL_BGRA;
			sizedFormat = GL_RGBA8;
			uncompressedType = GL_UNSIGNED_BYTE;
			blockOrPixelSize = 4;
		}
		else if (memcmp(&header->ddspf, &DirectX::DDSPF_A8B8G8R8, sizeof(DirectX::DDS_PIXELFORMAT)) == 0 ||
			memcmp(&header->ddspf, &DirectX::DDSPF_X8B8G8R8, sizeof(DirectX::DDS_PIXELFORMAT)) == 0)
		{
			compressed = false;
			format = GL_RGBA;
			sizedFormat = GL_RGBA8;
			uncompressedType = GL_UNSIGNED_BYTE;
			blockOrPixelSize = 4;
		}
		else
		{
			assert(!"Format nor supported");
			return (false);
		}
		return (true);
	}
}