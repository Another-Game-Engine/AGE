#pragma once

#include <Configuration.hpp>
#include <imgui/imgui.h>
#include <Utils/OpenGL.hh>
#include <Utils/DependenciesInjector.hpp>
#include <vector>

namespace AGE
{
	struct Age_ImDrawList
	{
		std::vector<ImDrawCmd>     commands;
		std::vector<ImDrawVert>    vtx_buffer;
		Age_ImDrawList(const ImDrawList& o)
		{
			const ImDrawCmd* pcmd_end = o.commands.end();
			for (const ImDrawCmd* pcmd = o.commands.begin(); pcmd != pcmd_end; pcmd++)
			{
				commands.push_back(*pcmd);
			}
			auto ve = o.vtx_buffer.end();
			for (auto v = o.vtx_buffer.begin(); v != ve; v++)
			{
				vtx_buffer.push_back(*v);
			}
		}
	};

	struct RenderImgui
	{
		std::vector<Age_ImDrawList> cmd_lists;
		RenderImgui(ImDrawList** const _cmd_lists, int _cmd_lists_count)
		{
			for (auto i = 0; i < _cmd_lists_count; ++i)
				cmd_lists.push_back(*_cmd_lists[i]);
		}

		//RenderImgui(const RenderImgui &o)
		//{
		//	cmd_lists = o.cmd_lists;
		//}

		//RenderImgui &operator=(const RenderImgui &o)
		//{
		//	cmd_lists = o.cmd_lists;
		//}

		//RenderImgui(RenderImgui &&o)
		//{
		//	cmd_lists = std::move(o.cmd_lists);
		//}

		//	RenderImgui &operator=(RenderImgui &&o)
		//{
		//	cmd_lists = std::move(o.cmd_lists);
		//}
	};


	class Imgui
	{
		static unsigned int _fontTex;
		static int _shader_handle, _vert_handle, _frag_handle;
		static int _texture_location, _ortho_location;
		static int _position_location, _uv_location, _colour_location;
		static unsigned int _vbohandle, _cursor, _size;
		DependenciesInjector *_dependencyInjector = nullptr;
		bool _releaseWork = false;
		bool _launched = false;
	public:
		Imgui();
		bool init(DependenciesInjector *di);
		void startUpdate();
		static Imgui* getInstance();
		static void renderDrawLists(ImDrawList** const cmd_lists, int cmd_lists_count);
		static void initShader(int *pid, int *vert, int *frag, const char *vs, const char *fs);
		void renderThreadRenderFn(const std::vector<Age_ImDrawList> &cmd_lists);

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