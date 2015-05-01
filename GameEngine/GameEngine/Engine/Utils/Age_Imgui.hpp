#pragma once

#include <Configuration.hpp>
#include <imgui/imgui.h>
#include <Utils/OpenGL.hh>
#include <Utils/DependenciesInjector.hpp>
#include <Core/Inputs/AgeInputs.hh>
#include <glm/glm.hpp>
#include <vector>

namespace AGE
{
	class Engine;

	struct ImGuiKeyEvent
	{
		AgeKeys key;
		bool down;
		ImGuiKeyEvent(AgeKeys _key, bool _down)
			: key(_key)
			, down(_down)
		{}
	};
	
	struct ImGuiMouseStateEvent
	{
		glm::ivec2 mousePosition;
		bool mouseState[3];
		float mouseWheel;

		ImGuiMouseStateEvent() :
			mousePosition(0)
			, mouseWheel(0.0f)
		{
			mouseState[0] = false;
			mouseState[1] = false;
			mouseState[2] = false;
		}

		ImGuiMouseStateEvent(glm::ivec2 const &_mousePosition, bool leftClic, bool wheelClic, bool rightClic, float _mouseWheel)
			: mousePosition(_mousePosition)
			, mouseWheel(_mouseWheel)
		{
			mouseState[0] = leftClic;
			mouseState[1] = wheelClic;
			mouseState[2] = rightClic;
		}
	};

	struct Age_ImDrawList
	{
		std::vector<ImDrawCmd>     commands;
		std::vector<ImDrawVert>    vtx_buffer;
		Age_ImDrawList() = default;
		Age_ImDrawList(const Age_ImDrawList& o)
		{
			commands = o.commands;
			vtx_buffer = o.vtx_buffer;
		}
		Age_ImDrawList& operator=(const Age_ImDrawList& o)
		{
			commands = o.commands;
			vtx_buffer = o.vtx_buffer;
			return *this;
		}
		Age_ImDrawList(const ImDrawList& o)
		{
			const ImDrawCmd* pcmd_end = o.commands.end();
			commands.resize(o.commands.size());
			int i = 0;
			for (const ImDrawCmd* pcmd = o.commands.begin(); pcmd != pcmd_end; pcmd++)
			{
				commands[i] = (*pcmd);
				++i;
			}
			auto ve = o.vtx_buffer.end();
			vtx_buffer.resize(o.vtx_buffer.size());
			i = 0;
			for (auto v = o.vtx_buffer.begin(); v != ve; v++)
			{
				vtx_buffer[i] = *v;
				++i;
			}
		}
	};

	struct RenderImgui
	{
		std::vector<Age_ImDrawList> cmd_lists;
		RenderImgui(ImDrawList** const _cmd_lists, int _cmd_lists_count)
		{
			cmd_lists.resize(_cmd_lists_count);
			for (auto i = 0; i < _cmd_lists_count; ++i)
				cmd_lists[i] = Age_ImDrawList(*_cmd_lists[i]);
		}

		RenderImgui(const RenderImgui &o)
		{
			cmd_lists = o.cmd_lists;
		}

		RenderImgui &operator=(const RenderImgui &o)
		{
			cmd_lists = o.cmd_lists;
			return *this;
		}

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
		static int shader_handle, vert_handle, frag_handle;
		static int texture_location, ortho_location;
		static int position_location, uv_location, colour_location;
		static size_t vbo_max_size;
		static unsigned int vbo_handle, vao_handle;
		static GLuint fontTex;

		Engine *_engine = nullptr;
		bool _releaseWork = false;
		bool _launched = false;
		ImGuiMouseStateEvent _lastMouseState;
	public:
		Imgui();
		bool init(Engine *en);
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