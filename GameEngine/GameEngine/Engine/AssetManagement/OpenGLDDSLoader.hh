#pragma once

#include <Utils/OldFile.hpp>
#include <Render/Textures/Texture2D.hh>
#include <Render/Textures/TextureCubeMap.hh>
#include <DirectXTex/DirectXTex/DDS.h>
#include <memory>

namespace AGE
{
	class OpenGLDDSLoader
	{
	public:
		static std::shared_ptr<ATexture> loadDDSFile(OldFile const &file);
	private:
		static bool getTexturePixelInfos(DirectX::DDS_HEADER const *header, bool &compressed,
			size_t &blockOrPixelSize, GLenum &format, GLenum &sizedFormat, GLenum &uncompressedType);
	};
}

