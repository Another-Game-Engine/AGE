#pragma once

#include <Configuration.hpp>
#include <imgui/imgui.h>
#include <Utils/OpenGL.hh>
#include <Utils/DependenciesInjector.hpp>

namespace AGE
{
	class Imgui
	{
		static unsigned int _fontTex;
		static int _shader_handle, _vert_handle, _frag_handle;
		static int _texture_location, _ortho_location;
		static int _position_location, _uv_location, _colour_location;
		static unsigned int _vbohandle, _cursor, _size;
		bool _releaseWork = false;
		bool _launched = false;
	public:
		Imgui();
		bool init(DependenciesInjector *di);
		void startUpdate();
		static Imgui* getInstance();
		static void renderDrawLists(ImDrawList** const cmd_lists, int cmd_lists_count);
		static void initShader(int *pid, int *vert, int *frag, const char *vs, const char *fs);

		template<typename T>
		static unsigned int stream(GLenum target, unsigned int vbo, unsigned int *vbo_cursor, unsigned int *vbo_size, T *start, int elementCount)
		{
			unsigned int bytes = sizeof(T) *elementCount;
			unsigned int aligned = bytes + bytes % 64; //align memory

			glBindBuffer(target, vbo);
			//If there's not enough space left, orphan the buffer object, create a new one and start writing
			if (vbo_cursor[0] + aligned > vbo_size[0])
			{
				assert(aligned < vbo_size[0]);
				glBufferData(target, vbo_size[0], NULL, GL_DYNAMIC_DRAW);
				vbo_cursor[0] = 0;
			}

			void* mapped = glMapBufferRange(target, vbo_cursor[0], aligned, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);

			memcpy(mapped, start, bytes);
			vbo_cursor[0] += aligned;

			glUnmapBuffer(target);
			return vbo_cursor[0] - aligned; //return the offset we use for glVertexAttribPointer call
		}

	};
}