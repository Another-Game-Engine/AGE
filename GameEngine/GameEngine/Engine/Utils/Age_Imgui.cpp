#include "Age_Imgui.hpp"
#include <Context/IRenderContext.hh>
#include <SDL/SDL_keycode.h>
#include <imgui/imconfig.h>
#include <Utils/Utils.hh>
#include <Threads/PrepareRenderThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Threads/MainThread.hpp>
#include <Core/Engine.hh>
#include <Core/Input.hh>
#define STB_TRUETYPE_IMPLEMENTATION
#include <imgui/stb_truetype.h>
#ifdef _MSC_VER
#pragma warning (disable: 4996)         // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#endif
#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

namespace AGE
{
	size_t Imgui::vbo_max_size = 20000;
	int Imgui::shader_handle, Imgui::vert_handle, Imgui::frag_handle;
	int Imgui::texture_location, Imgui::ortho_location;
	int Imgui::position_location, Imgui::uv_location, Imgui::colour_location;
	unsigned int Imgui::vbo_handle, Imgui::vao_handle;
	GLuint Imgui::fontTex;

	bool Imgui::init(Engine *en)
	{
#ifdef USE_IMGUI


		GetMainThread()->registerCallback<ImGuiKeyEvent>([this](ImGuiKeyEvent &msg)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.KeysDown[msg.key.scancode] = msg.down;
			if (msg.down)
			{
				if ((msg.key.sym >= 33 && msg.key.sym <= 126))
					io.AddInputCharacter(msg.key.sym);
			}
			io.KeyCtrl = msg.down && ((msg.key.mod & KMOD_LCTRL || msg.key.mod & KMOD_RCTRL));
			io.KeyShift = msg.down && ((msg.key.mod & KMOD_SHIFT));
		});

		_engine = en;
		//HARDCODED WINDOW TO FIX
		//auto window = di->getInstance<AGE::Threads::Render>()->getCommandQueue()->safePriorityFutureEmplace<RendCtxCommand::GetScreenSize, glm::uvec2>().get();

		ImGuiIO& io = ImGui::GetIO();
		//HARDCODED WINDOW TO FIX
		auto screenSize = en->getInstance<IRenderContext>()->getScreenSize();
		io.DisplaySize = ImVec2((float)screenSize.x, (float)screenSize.y);        // Display size, in pixels. For clamping windows positions.
		io.DeltaTime = 1.0f / 60.0f;                          // Time elapsed since last frame, in seconds (in this sample app we'll override this every frame because our timestep is variable)
		io.KeyMap[ImGuiKey_Tab] = SDL_SCANCODE_TAB;             // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
		io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
		io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
		io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
		io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
		io.KeyMap[ImGuiKey_Delete] = SDL_SCANCODE_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
		io.KeyMap[ImGuiKey_Enter] = SDL_SCANCODE_RETURN;
		io.KeyMap[ImGuiKey_Escape] = SDL_SCANCODE_ESCAPE;
		io.KeyMap[ImGuiKey_A] = SDL_SCANCODE_A;
		io.KeyMap[ImGuiKey_C] = SDL_SCANCODE_C;
		io.KeyMap[ImGuiKey_V] = SDL_SCANCODE_V;
		io.KeyMap[ImGuiKey_X] = SDL_SCANCODE_X;
		io.KeyMap[ImGuiKey_Y] = SDL_SCANCODE_Y;
		io.KeyMap[ImGuiKey_Z] = SDL_SCANCODE_Z;

		io.RenderDrawListsFn = renderDrawLists;

		//io.SetClipboardTextFn = ImImpl_SetClipboardTextFn;
		//io.GetClipboardTextFn = ImImpl_GetClipboardTextFn;

		const GLchar *vertex_shader =
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
			"	gl_Position = ortho*vec4(Position.xy,0,1);\n"
			"}\n";

		const GLchar* fragment_shader =
			"#version 330\n"
			"uniform sampler2D Texture;\n"
			"in vec2 Frag_UV;\n"
			"in vec4 Frag_Colour;\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"	FragColor = Frag_Colour * texture( Texture, Frag_UV.st);\n"
			"}\n";

		shader_handle = glCreateProgram();
		vert_handle = glCreateShader(GL_VERTEX_SHADER);
		frag_handle = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(vert_handle, 1, &vertex_shader, 0);
		glShaderSource(frag_handle, 1, &fragment_shader, 0);
		glCompileShader(vert_handle);
		glCompileShader(frag_handle);
		glAttachShader(shader_handle, vert_handle);
		glAttachShader(shader_handle, frag_handle);
		glLinkProgram(shader_handle);

		texture_location = glGetUniformLocation(shader_handle, "Texture");
		ortho_location = glGetUniformLocation(shader_handle, "ortho");
		position_location = glGetAttribLocation(shader_handle, "Position");
		uv_location = glGetAttribLocation(shader_handle, "UV");
		colour_location = glGetAttribLocation(shader_handle, "Colour");

		glGenBuffers(1, &vbo_handle);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
		glBufferData(GL_ARRAY_BUFFER, vbo_max_size, NULL, GL_DYNAMIC_DRAW);

		glGenVertexArrays(1, &vao_handle);
		glBindVertexArray(vao_handle);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
		glEnableVertexAttribArray(position_location);
		glEnableVertexAttribArray(uv_location);
		glEnableVertexAttribArray(colour_location);

		glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
		glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
		glVertexAttribPointer(colour_location, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Load font texture
		glGenTextures(1, &fontTex);
		glBindTexture(GL_TEXTURE_2D, fontTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		unsigned char* pixels;
		int width, height;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
		io.Fonts->TexID = &fontTex;

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
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

		auto input = this->_engine->getInstance<AGE::Input>();

		// Setup timestep
		static double time = 0.0f;
		const double current_time = SDL_GetTicks();
		float dif = (float)(current_time - time);
		io.DeltaTime = dif == 0.0f ? 0.000000001f : dif / 1000.0f;
		time = current_time;

		int mx, my;
		auto mouseState = SDL_GetMouseState(&mx, &my);
		io.MousePos = ImVec2((float)mx, (float)my);
		io.MouseDown[0] = (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
		io.MouseDown[1] = (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
		io.MouseDown[2] = (mouseState & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;

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
		AGE::GetPrepareThread()->getQueue()->emplaceCommand<AGE::RenderImgui>(cmd_lists, cmd_lists_count);
#else
		UNUSED(cmd_lists);
		UNUSED(cmd_lists_count);
#endif
	}

	void Imgui::renderThreadRenderFn(std::vector<Age_ImDrawList> const &cmd_lists)
	{

		if (cmd_lists.empty())
			return;
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_SCISSOR_TEST);

		// Setup texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fontTex);

		// Setup orthographic projection matrix
		const float width = ImGui::GetIO().DisplaySize.x;
		const float height = ImGui::GetIO().DisplaySize.y;
		const float ortho_projection[4][4] =
		{
			{ 2.0f / width, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 2.0f / -height, 0.0f, 0.0f },
			{ 0.0f, 0.0f, -1.0f, 0.0f },
			{ -1.0f, 1.0f, 0.0f, 1.0f },
		};
		glUseProgram(shader_handle);
		glUniform1i(texture_location, 0);
		glUniformMatrix4fv(ortho_location, 1, GL_FALSE, &ortho_projection[0][0]);

		// Grow our buffer according to what we need
		size_t total_vtx_count = 0;
		for (int n = 0; n < cmd_lists.size(); n++)
			total_vtx_count += cmd_lists[n].vtx_buffer.size();
		glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
		size_t neededBufferSize = total_vtx_count * sizeof(ImDrawVert);
		if (neededBufferSize > vbo_max_size)
		{
			vbo_max_size = neededBufferSize + 5000;  // Grow buffer
			glBufferData(GL_ARRAY_BUFFER, vbo_max_size, NULL, GL_STREAM_DRAW);
		}

		// Copy and convert all vertices into a single contiguous buffer
		unsigned char* buffer_data = (unsigned char*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		if (!buffer_data)
			return;
		for (int n = 0; n < cmd_lists.size(); n++)
		{
			auto &cmd_list = cmd_lists[n];
			memcpy(buffer_data, &cmd_list.vtx_buffer[0], cmd_list.vtx_buffer.size() * sizeof(ImDrawVert));
			buffer_data += cmd_list.vtx_buffer.size() * sizeof(ImDrawVert);
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(vao_handle);

		int cmd_offset = 0;
		for (int n = 0; n < cmd_lists.size(); n++)
		{
			auto &cmd_list = cmd_lists[n];
			int vtx_offset = cmd_offset;
			auto &pcmd_end = std::end(cmd_list.commands);
			for (auto &pcmd = std::begin(cmd_list.commands); pcmd != pcmd_end; pcmd++)
			{
				glScissor((int)pcmd->clip_rect.x, (int)(height - pcmd->clip_rect.w), (int)(pcmd->clip_rect.z - pcmd->clip_rect.x), (int)(pcmd->clip_rect.w - pcmd->clip_rect.y));
				glDrawArrays(GL_TRIANGLES, vtx_offset, pcmd->vtx_count);
				vtx_offset += pcmd->vtx_count;
			}
			cmd_offset = vtx_offset;
		}

		// Restore modified state
		glBindVertexArray(0);
		glUseProgram(0);
		glDisable(GL_SCISSOR_TEST);
		glBindTexture(GL_TEXTURE_2D, 0);
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