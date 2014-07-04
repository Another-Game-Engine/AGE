#include "CubeMapFile.hpp"
#include "AssetsManager.hpp"
#include <OpenGL/ShadingManager.hh>

CubeMapFile::CubeMapFile()
: MediaFile<CubeMapFile>()
, px(nullptr)
, nx(nullptr)
, py(nullptr)
, ny(nullptr)
, pz(nullptr)
, nz(nullptr)
, _id(0)
, _vbo(0)
, _vao(0)
{
	_type = CUBEMAP;
}

CubeMapFile::~CubeMapFile()
{
	if (_id != 0)
		glDeleteTextures(1, &_id);
	if (_vao != 0)
		glDeleteVertexArrays(1, &_vao);
	if (_vbo != 0)
		glDeleteBuffers(1, &_vbo);
}

CubeMapFile::CubeMapFile(const CubeMapFile &o)
: MediaFile<CubeMapFile>(o)
, px(nullptr)
, nx(nullptr)
, py(nullptr)
, ny(nullptr)
, pz(nullptr)
, nz(nullptr)
, _id(0)
{
	px = o.px;
	nx = o.nx;
	py = o.py;
	ny = o.ny;
	pz = o.pz;
	nz = o.nz;
	init();
}

CubeMapFile &CubeMapFile::operator=(const CubeMapFile &o)
{
	if (&o != this)
	{
		px = o.px;
		nx = o.nx;
		py = o.py;
		ny = o.ny;
		pz = o.pz;
		nz = o.nz;
	}
	return *this;
}


void CubeMapFile::save(cereal::PortableBinaryOutputArchive &ar) const
{
	ar(px->path.getFullName(), py->path.getFullName(), pz->path.getFullName(),
		nx->path.getFullName(), ny->path.getFullName(), nz->path.getFullName());
}

void CubeMapFile::load(cereal::PortableBinaryInputArchive &ar)
{
	std::string _px, _py, _pz, _nx, _ny, _nz;
	ar(_px, _py, _pz, _nx, _ny, _nz);
	auto assetsManager = _dpyManager.lock()->getInstance<AssetsManager>();
	auto manager = _dpyManager.lock()->getInstance<gl::ShadingManager>();
	if (manager)
	{
		px = std::static_pointer_cast<TextureFile>(assetsManager->loadFromFile(File(_px), *manager));
		py = std::static_pointer_cast<TextureFile>(assetsManager->loadFromFile(File(_py), *manager));
		pz = std::static_pointer_cast<TextureFile>(assetsManager->loadFromFile(File(_pz), *manager));
		nx = std::static_pointer_cast<TextureFile>(assetsManager->loadFromFile(File(_nx), *manager));
		ny = std::static_pointer_cast<TextureFile>(assetsManager->loadFromFile(File(_ny), *manager));
		nz = std::static_pointer_cast<TextureFile>(assetsManager->loadFromFile(File(_nz), *manager));
	}
	init();
}

void CubeMapFile::init()
{
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &_id);
	assert(_id > 0 && "glGenTextures error.");
	glBindTexture(GL_TEXTURE_CUBE_MAP, _id);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, px->components, px->width, px->height, 0, px->format, GL_UNSIGNED_BYTE, px->datas.data());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, nx->components, nx->width, nx->height, 0, nx->format, GL_UNSIGNED_BYTE, nx->datas.data());

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, py->components, py->width, py->height, 0, py->format, GL_UNSIGNED_BYTE, py->datas.data());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, ny->components, ny->width, ny->height, 0, ny->format, GL_UNSIGNED_BYTE, ny->datas.data());

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, pz->components, pz->width, pz->height, 0, pz->format, GL_UNSIGNED_BYTE, pz->datas.data());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, nz->components, nz->width, nz->height, 0, nz->format, GL_UNSIGNED_BYTE, nz->datas.data());

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	// build cube
	float points[] = {
		-10.0f, 10.0f, -10.0f,
		-10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, 10.0f, -10.0f,
		-10.0f, 10.0f, -10.0f,

		-10.0f, -10.0f, 10.0f,
		-10.0f, -10.0f, -10.0f,
		-10.0f, 10.0f, -10.0f,
		-10.0f, 10.0f, -10.0f,
		-10.0f, 10.0f, 10.0f,
		-10.0f, -10.0f, 10.0f,

		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,

		-10.0f, -10.0f, 10.0f,
		-10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, -10.0f, 10.0f,
		-10.0f, -10.0f, 10.0f,

		-10.0f, 10.0f, -10.0f,
		10.0f, 10.0f, -10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		-10.0f, 10.0f, 10.0f,
		-10.0f, 10.0f, -10.0f,

		-10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f, 10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f, 10.0f,
		10.0f, -10.0f, 10.0f
	};

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof (float), &points, GL_STATIC_DRAW);

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

}


void CubeMapFile::draw() const
{
	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}