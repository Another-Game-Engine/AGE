#include "Age_Imgui.hpp"
#include <Context/IRenderContext.hh>
#include <Render/RenderThreadInterface.hpp>
#include <SDL/SDL_keycode.h>
#include <SDL/SDL.h>
#include <imgui/imconfig.h>
#include <Utils/Utils.hh>

#define STB_IMAGE_IMPLEMENTATION
#include <imgui\stb_image.h>
#ifdef _MSC_VER
#pragma warning (disable: 4996)         // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#endif

namespace AGE
{

	unsigned int Imgui::_fontTex = 0;
	int Imgui::_shader_handle = 0;
	int Imgui::_vert_handle = 0;
	int Imgui::_frag_handle = 0;
	int Imgui::_texture_location = 0;
	int Imgui::_ortho_location = 0;
	int Imgui::_position_location = 0;
	int Imgui::_uv_location = 0;
	int Imgui::_colour_location = 0;
	unsigned int Imgui::_vbohandle = 0;
	unsigned int Imgui::_cursor = 0;
	unsigned int Imgui::_size = 0;

	bool Imgui::init(DependenciesInjector *di)
	{
#ifdef USE_IMGUI
		//HARDCODED WINDOW TO FIX
		//auto window = di->getInstance<AGE::Threads::Render>()->getCommandQueue().safePriorityFutureEmplace<RendCtxCommand::GetScreenSize, glm::uvec2>().get();

		ImGuiIO& io = ImGui::GetIO();
		//HARDCODED WINDOW TO FIX
		//io.DisplaySize = ImVec2((float)window.x, (float)window.y);        // Display size, in pixels. For clamping windows positions.
		io.DisplaySize = ImVec2(800, 600);        // Display size, in pixels. For clamping windows positions.
		io.DeltaTime = 1.0f / 60.0f;                          // Time elapsed since last frame, in seconds (in this sample app we'll override this every frame because our timestep is variable)
		io.PixelCenterOffset = 0.0f;                        // Align OpenGL texels
		io.KeyMap[ImGuiKey_Tab] = SDLK_TAB;             // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
		io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
		io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
		io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
		io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
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

		io.RenderDrawListsFn = [](ImDrawList** const draw_lists, int count){

		};
//		renderDrawLists;

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

		static const char vertex_shader[] = \
			"#version 330\n"
			"uniform mat4 ortho;\n"

			"in vec2 Position;\n"
			"in vec2 UV;\n"
			"in vec4 Colour;\n"

			"out vec2 Frag_UV;\n"
			"out vec4 Frag_Colour;\n"

			"void main()\n"
			"{\n"
			"	Frag_UV = UV;\n"
			"	Frag_Colour = Colour;\n"
			"\n"
			"	gl_Position = ortho*vec4(Position.xy,0,1);\n"
			"}\n";

		static const char fragment_shader[] = \
			"#version 330\n"
			"uniform sampler2D Texture;\n"

			"in vec2 Frag_UV;\n"
			"in vec4 Frag_Colour;\n"

			"out vec4 FragColor;\n"

			"void main()\n"
			"{\n"
			"	FragColor = Frag_Colour * texture( Texture, Frag_UV.st);\n"
			"}\n";

		initShader(&_shader_handle, &_vert_handle, &_frag_handle, vertex_shader, fragment_shader);

		_texture_location = glGetUniformLocation(_shader_handle, "Texture");
		_ortho_location = glGetUniformLocation(_shader_handle, "ortho");
		_position_location = glGetAttribLocation(_shader_handle, "Position");
		_uv_location = glGetAttribLocation(_shader_handle, "UV");
		_colour_location = glGetAttribLocation(_shader_handle, "Colour");

		_size = static_cast<int>(pow(2.0, 20.0)); //1Mb streaming buffer
		glGenBuffers(1, &_vbohandle);
		glBindBuffer(GL_ARRAY_BUFFER, _vbohandle);
		glBufferData(GL_ARRAY_BUFFER, _size, NULL, GL_DYNAMIC_DRAW);
#else
		UNUSED(di);
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

	Imgui* Imgui::getInstance()
	{
		static Imgui* ImguiInstance = new Imgui();
		return ImguiInstance;
	}

	static void make_ortho(float *result, float const & left, float const & right, float const & bottom, float const & top, float const & zNear, float const & zFar)
	{
#ifdef USE_IMGUI
		result[0] = static_cast<float>(2) / (right - left);
		result[5] = static_cast<float>(2) / (top - bottom);
		result[10] = -float(2) / (zFar - zNear);
		result[12] = -(right + left) / (right - left);
		result[13] = -(top + bottom) / (top - bottom);
		result[14] = -(zFar + zNear) / (zFar - zNear);
#else
		UNUSED(result);
		UNUSED(left);
		UNUSED(right);
		UNUSED(bottom);
		UNUSED(top);
		UNUSED(zNear);
		UNUSED(zFar);
#endif
	}

	void Imgui::renderDrawLists(ImDrawList** const cmd_lists, int cmd_lists_count)
	{
#ifdef USE_IMGUI
		if (cmd_lists_count == 0)
			return;

		// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_SCISSOR_TEST);

		// Setup texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _fontTex);

		// Setup orthographic projection matrix
		const float width = ImGui::GetIO().DisplaySize.x;
		const float height = ImGui::GetIO().DisplaySize.y;
		float ortho[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 }; //identity matrix
		make_ortho(ortho, 0.0f, width, height, 0.0f, -1.0f, +1.0f);

		glUseProgram(_shader_handle);
		glUniform1i(_texture_location, 0);
		glUniformMatrix4fv(_ortho_location, 1, GL_FALSE, ortho);

		glEnableVertexAttribArray(_position_location);
		glEnableVertexAttribArray(_uv_location);
		glEnableVertexAttribArray(_colour_location);

		for (int n = 0; n < cmd_lists_count; n++)
		{
			const ImDrawList* cmd_list = cmd_lists[n];
			const const ImDrawVert* vtx_buffer = reinterpret_cast<const ImDrawVert*>(cmd_list->vtx_buffer.begin());
			int vtx_size = static_cast<int>(cmd_list->vtx_buffer.size());
		
			unsigned offset = stream(GL_ARRAY_BUFFER, _vbohandle, &_cursor, &_size, vtx_buffer, vtx_size);
		
			glVertexAttribPointer(_position_location, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (void*)(offset));
			glVertexAttribPointer(_uv_location, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (void*)(offset + 8));
			glVertexAttribPointer(_colour_location, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (void*)(offset + 16));
		
			int vtx_offset = 0;
			const ImDrawCmd* pcmd_end = cmd_list->commands.end();
			for (const ImDrawCmd* pcmd = cmd_list->commands.begin(); pcmd != pcmd_end; pcmd++)
			{
				glScissor((int)pcmd->clip_rect.x, (int)(height - pcmd->clip_rect.w), (int)(pcmd->clip_rect.z - pcmd->clip_rect.x), (int)(pcmd->clip_rect.w - pcmd->clip_rect.y));
				glDrawArrays(GL_TRIANGLES, vtx_offset, pcmd->vtx_count);
				vtx_offset += pcmd->vtx_count;
			}
		}

		glDisableVertexAttribArray(_position_location);
		glDisableVertexAttribArray(_uv_location);
		glDisableVertexAttribArray(_colour_location);
		glUseProgram(0);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_SCISSOR_TEST);
		glDisable(GL_BLEND);
#else
		UNUSED(cmd_lists);
		UNUSED(cmd_lists_count);
#endif
	}

	void Imgui::initShader(int *pid, int *vert, int *frag, const char *vs, const char *fs)
	{
#ifdef USE_IMGUI
		*pid = glCreateProgram();
		*vert = glCreateShader(GL_VERTEX_SHADER);
		*frag = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(*vert, 1, &vs, 0);
		glShaderSource(*frag, 1, &fs, 0);
		glCompileShader(*vert);
		glCompileShader(*frag);
		glAttachShader(*pid, *vert);
		glAttachShader(*pid, *frag);
		glLinkProgram(*pid);
#else
		UNUSED(pid);
		UNUSED(vert);
		UNUSED(frag);
		UNUSED(vs);
		UNUSED(fs);
#endif
	}
}