#include <string>

char const *quad_shader_vertex = 
"#version 440											\n \
														\
 const vec2 madd = vec2(0.5f, 0.5f);					\n \
														\
 layout (location = 0) in vec2 position;				\n \
														\
 out vec2 interpolated_texCoord;						\n \
														\
 void main()											\n \
 {														\n \
    interpolated_texCoord = position * madd + madd;		\n \
    gl_Position = vec4(position, 0.0f, 1.0f);			\n \
 }														\n ";

char const *quad_shader_fragment = 
"#version 440													\n \
																\
 in vec2 interpolated_texCoord;									\n \
																\
 layout (location = 0) out vec4 colorOutput;					\n \
																\
 void main()													\n \
 {																\n \
 	colorOutput = texture2D(texture, interpolated_texCoord);	\n \
 }																\n";
