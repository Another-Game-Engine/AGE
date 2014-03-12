#ifndef SHADER_HH_
# define SHADER_HH_

# include <map>
# include <vector>
# include <set>

#include "AShader.hh"

namespace OpenGLTools
{
	class Shader : public AShader
	{
	private:
		GLuint						_vertexId;
		GLuint						_fragId;
		GLuint						_geometryId;

		GLenum                      *_targets;
		GLuint                      _textureNumber;
		std::set<GLenum>            _targetsList;
		std::set<GLenum>            _layersList;

	public:
		Shader(void);
		virtual ~Shader(void);

		bool	init(std::string const &vertex, std::string const &fragment, std::string const &geometry = "");

		GLenum  *getTargets() const;
		std::size_t getTargetsNumber() const;

		Shader &addTarget(GLenum target);
		Shader &removeTarget(GLenum target);
		void clearTargets();
		Shader &addLayer(GLenum layer);
		Shader &removeLayer(GLenum layer);
		void clearLayers();
		inline const std::set<GLenum> &getLayers() const {return _layersList;}
		inline Shader &setTextureNumber(unsigned int t){_textureNumber = t; return *this;}
	private:
		bool _build();
	};

}

#endif /*!SHADER_HH_*/
