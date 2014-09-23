#include "Age_Imgui.hpp"
#include <Context/IRenderContext.hh>
#include <Render/RenderThreadInterface.hpp>
#include <Utils/DependenciesInjector.hpp>
#include <SDL/SDL_keycode.h>
#include <SDL/SDL.h>
#include <Utils/OpenGL.hh>
#include <imgui/imconfig.h>

#define STB_IMAGE_IMPLEMENTATION
#include <imgui\stb_image.h>
#ifdef _MSC_VER
#pragma warning (disable: 4996)         // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#endif

namespace AGE
{

	unsigned int Imgui::_fontTex = 0;

	bool Imgui::init(DependenciesInjector *di)
	{
#ifdef USE_IMGUI
		auto window = di->getInstance<AGE::Threads::Render>()->getCommandQueue().safePriorityFutureEmplace<RendCtxCommand::GetScreenSize, glm::uvec2>().get();

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)window.x, (float)window.y);        // Display size, in pixels. For clamping windows positions.
		io.DeltaTime = 1.0f / 60.0f;                          // Time elapsed since last frame, in seconds (in this sample app we'll override this every frame because our timestep is variable)
		io.PixelCenterOffset = 0.0f;                        // Align OpenGL texels
		io.KeyMap[ImGuiKey_Tab] = SDLK_TAB;             // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
		io.KeyMap[ImGuiKey_LeftArrow] = SDLK_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = SDLK_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = SDLK_UP;
		io.KeyMap[ImGuiKey_DownArrow] = SDLK_DOWN;
		io.KeyMap[ImGuiKey_Home] = SDLK_HOME;
		io.KeyMap[ImGuiKey_End] = SDLK_END;
		io.KeyMap[ImGuiKey_Delete] = SDLK_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = SDLK_BACKSPACE;
		io.KeyMap[ImGuiKey_Enter] = SDLK_RETURN;
		io.KeyMap[ImGuiKey_Escape] = SDLK_ESCAPE;
		io.KeyMap[ImGuiKey_A] = SDLK_a;
		io.KeyMap[ImGuiKey_C] = SDLK_c;
		io.KeyMap[ImGuiKey_V] = SDLK_v;
		io.KeyMap[ImGuiKey_X] = SDLK_x;
		io.KeyMap[ImGuiKey_Y] = SDLK_y;
		io.KeyMap[ImGuiKey_Z] = SDLK_z;

		io.RenderDrawListsFn = renderDrawLists;
		//io.SetClipboardTextFn = ImImpl_SetClipboardTextFn;
		//io.GetClipboardTextFn = ImImpl_GetClipboardTextFn;

		//// Load font texture
		glGenTextures(1, &_fontTex);
		glBindTexture(GL_TEXTURE_2D, _fontTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		const void* png_data;
		unsigned int png_size;
		ImGui::GetDefaultFontData(NULL, NULL, &png_data, &png_size);
		int tex_x, tex_y, tex_comp;
		void* tex_data = stbi_load_from_memory((const unsigned char*)png_data, (int)png_size, &tex_x, &tex_y, &tex_comp, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_x, tex_y, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
		stbi_image_free(tex_data);

#endif //USE_IMGUI
		return true;
	}

	Imgui::Imgui()
	{
	}

	void Imgui::startUpdate()
	{
#ifdef USE_IMGUI
		ImGuiIO& io = ImGui::GetIO();

		// Setup timestep
		static double time = 0.0f;
		const double current_time = SDL_GetTicks();
		float dif = current_time - time;
		io.DeltaTime = dif == 0.0f ? 0.000000001f : dif / 1000.0f;
		time = current_time;

		// Setup inputs
		// (we already got mouse wheel, keyboard keys & characters from glfw callbacks polled in glfwPollEvents())
		int mx, my;
		auto mouseState = SDL_GetMouseState(&mx, &my);
		io.MousePos = ImVec2((float)mx, (float)my);
		io.MouseDown[0] = mouseState & SDL_BUTTON(SDL_BUTTON_LEFT);
		io.MouseDown[1] = mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT);
		io.MouseDown[2] = mouseState & SDL_BUTTON(SDL_BUTTON_MIDDLE);

		// Start the frame
		ImGui::NewFrame();
#endif
	}

	void Imgui::endUpdate()
	{
#ifdef USE_IMGUI
		for (auto &q : _commandQueue)
		{
			q.second->releaseReadability();
			q.second->getDispatcher()
				.handle<TQC::VoidFunction>([&](const TQC::VoidFunction& msg)
			{
				msg.function();
			});
		}
#endif
	}

	void Imgui::push(const std::function<void()> &fn)
	{
#ifdef USE_IMGUI
		std::lock_guard<std::mutex> lock(_mutex);
		std::shared_ptr<TMQ::Queue> queue = nullptr;
		{
			auto it = _threadIds.find(std::this_thread::get_id().hash());
			assert(it != std::end(_threadIds) && "Thread is not registered.");
			queue = _commandQueue[it->second];
		}

		queue->emplace<TQC::VoidFunction>(std::move(fn));
#endif
	}

	void Imgui::push(std::function<void()> &&fn)
	{
#ifdef USE_IMGUI
		std::lock_guard<std::mutex> lock(_mutex);
		std::shared_ptr<TMQ::Queue> queue = nullptr;
		{
			auto it = _threadIds.find(std::this_thread::get_id().hash());
			assert(it != std::end(_threadIds) && "Thread is not registered.");
			queue = _commandQueue[it->second];
		}

		queue->emplace<TQC::VoidFunction>(std::move(fn));
#endif
	}

	void Imgui::registerThread(std::size_t priority)
	{
#ifdef USE_IMGUI
		std::lock_guard<std::mutex> lock(_mutex);
		auto threadId = std::this_thread::get_id().hash();
		assert(_threadIds.find(threadId) == std::end(_threadIds) && "Thread already registered.");
		assert(_commandQueue.find(priority) == std::end(_commandQueue) && "Priority already reserved.");
		_threadIds.insert(std::make_pair(threadId, priority));
		auto queue = std::make_shared<TMQ::Queue>();
		_commandQueue.insert(std::make_pair(priority, queue));
		queue->launch();
#endif
	}

	Imgui* Imgui::getInstance()
	{
		static Imgui* ImguiInstance = new Imgui();
		return ImguiInstance;
	}

	void Imgui::renderDrawLists(ImDrawList** const cmd_lists, int cmd_lists_count)
	{
		if (cmd_lists_count == 0)
			return;

		// We are using the OpenGL fixed pipeline to make the example code simpler to read!
		// A probable faster way to render would be to collate all vertices from all cmd_lists into a single vertex buffer.
		// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, vertex/texcoord/color pointers.
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_SCISSOR_TEST);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		// Setup texture
		glBindTexture(GL_TEXTURE_2D, Imgui::_fontTex);
		glEnable(GL_TEXTURE_2D);

		// Setup orthographic projection matrix
		const float width = ImGui::GetIO().DisplaySize.x;
		const float height = ImGui::GetIO().DisplaySize.y;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0f, width, height, 0.0f, -1.0f, +1.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// Render command lists
		for (int n = 0; n < cmd_lists_count; n++)
		{
			const ImDrawList* cmd_list = cmd_lists[n];
			const unsigned char* vtx_buffer = (const unsigned char*)cmd_list->vtx_buffer.begin();
			glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void*)(vtx_buffer));
			glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void*)(vtx_buffer + 8));
			glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert), (void*)(vtx_buffer + 16));

			int vtx_offset = 0;
			const ImDrawCmd* pcmd_end = cmd_list->commands.end();
			for (const ImDrawCmd* pcmd = cmd_list->commands.begin(); pcmd != pcmd_end; pcmd++)
			{
				glScissor((int)pcmd->clip_rect.x, (int)(height - pcmd->clip_rect.w), (int)(pcmd->clip_rect.z - pcmd->clip_rect.x), (int)(pcmd->clip_rect.w - pcmd->clip_rect.y));
				glDrawArrays(GL_TRIANGLES, vtx_offset, pcmd->vtx_count);
				vtx_offset += pcmd->vtx_count;
			}
		}
		glDisable(GL_SCISSOR_TEST);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glDisable(GL_BLEND);
	}

}