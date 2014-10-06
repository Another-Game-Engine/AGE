#pragma once

#include <Configuration.hpp>
#include <Utils/Dependency.hpp>
#include <imgui/imgui.h>
#include <Utils/ThreadQueueCommands.hpp>
#include <Utils/OpenGL.hh>
#include <queue>
#include <map>
#include <array>

namespace AGE
{
	class Imgui
	{
	private:

		struct ImguiCommand
		{
			std::function<void()> function;
			bool end = false;

			ImguiCommand(std::function<void()> &&f)
				: function(std::move(f))
				, end(false)
			{}

			ImguiCommand(bool _end)
				: end(_end)
			{}
		};

		std::map<std::size_t, std::size_t> _threadIds;
		std::map<std::size_t, std::queue<ImguiCommand>> _commandQueue;
		std::mutex _mutex;
		static unsigned int _fontTex;
		static int _shader_handle, _vert_handle, _frag_handle;
		static int _texture_location, _ortho_location;
		static int _position_location, _uv_location, _colour_location;
		static unsigned int _vbohandle, _cursor, _size;
		bool _releaseWork = false;
		bool _launched = false;
	public:
		Imgui();
		void launch() { _launched = true; }
		bool init(DependenciesInjector *di);
		void startUpdate();
		void endUpdate();
		void push(std::function<void()> &&fn);
		// Call this at the begining of a thread to register it to the Imgui command queue
		void registerThread(std::size_t priority);
		static Imgui* getInstance();
		static void renderDrawLists(ImDrawList** const cmd_lists, int cmd_lists_count);
		static void initShader(int *pid, int *vert, int *frag, const char *vs, const char *fs);
		void threadLoopEnd();


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

#ifdef USE_IMGUI
#define IMGUI_BEGIN ::AGE::Imgui::getInstance()->push([=](){

#define IMGUI_END });
#else
#define IMGUI_BEGIN (void)([=](){

#define IMGUI_END });
#endif

}