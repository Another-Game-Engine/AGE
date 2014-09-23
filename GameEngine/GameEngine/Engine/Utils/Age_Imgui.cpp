#include "Age_Imgui.hpp"
#include <Context/IRenderContext.hh>
#include <Render/RenderThreadInterface.hpp>
#include <Utils/DependenciesInjector.hpp>
#include <SDL/SDL_keycode.h>
#include <imgui/imconfig.h>

namespace AGE
{
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

		//io.RenderDrawListsFn = ImImpl_RenderDrawLists;
		//io.SetClipboardTextFn = ImImpl_SetClipboardTextFn;
		//io.GetClipboardTextFn = ImImpl_GetClipboardTextFn;

		//// Load font texture
		//glGenTextures(1, &fontTex);
		//glBindTexture(GL_TEXTURE_2D, fontTex);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//const void* png_data;
		//unsigned int png_size;
		//ImGui::GetDefaultFontData(NULL, NULL, &png_data, &png_size);
		//int tex_x, tex_y, tex_comp;
		//void* tex_data = stbi_load_from_memory((const unsigned char*)png_data, (int)png_size, &tex_x, &tex_y, &tex_comp, 0);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_x, tex_y, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
		//stbi_image_free(tex_data);

#endif //USE_IMGUI
		return true;
	}

	Imgui::Imgui()
	{
	}

	void Imgui::update()
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

}