#include "Age_Imgui.hpp"
#include <Context/IRenderContext.hh>
#include <SDL/SDL_keycode.h>
#include <imgui/imconfig.h>
#include <Utils/Utils.hh>
#include <Threads/ThreadManager.hpp>
#include <Threads/MainThread.hpp>
#include <Threads/RenderThread.hpp>
#include <Core/Engine.hh>
#include <Core/Inputs/Input.hh>
#include <Core/Timer.hh>
#include <Render/OpenGLTask/OpenGLState.hh>
#include <Utils/Profiler.hpp>

#define STB_TRUETYPE_IMPLEMENTATION
#include <imgui/stb_truetype.h>
#ifdef _MSC_VER
#pragma warning (disable: 4996)         // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#endif
#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

namespace AGE
{
	size_t Imgui::vbo_max_size = 20000;
	size_t Imgui::vbo_size = 0;
	int Imgui::shader_handle, Imgui::vert_handle, Imgui::frag_handle;
	int Imgui::texture_location, Imgui::ortho_location;
	int Imgui::position_location, Imgui::uv_location, Imgui::colour_location;
	unsigned int Imgui::vbo_handle, Imgui::vao_handle;
	GLuint Imgui::fontTex;

	bool Imgui::init(Engine *en)
	{
#ifdef AGE_ENABLE_IMGUI

		GetMainThread()->registerCallback<ImGuiKeyEvent>([this](ImGuiKeyEvent &msg)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.KeysDown[msg.key] = msg.down;
			if (msg.key == AGE_LCTRL || msg.key == AGE_RCTRL)
				io.KeyCtrl = msg.down;
			else if (msg.key == AGE_LSHIFT || msg.key == AGE_RSHIFT)
				io.KeyShift = msg.down;
			else if (msg.down)
			{
				char character = keyToAscii.find(msg.key)->second;
				if (character != UNDEFINED_CHARACTER)
					io.AddInputCharacter(character);
			}
		});

		GetMainThread()->registerCallback<ImGuiMouseStateEvent>([this](ImGuiMouseStateEvent &msg)
		{
			_lastMouseState = msg;
		});

		_engine = en;
		//HARDCODED WINDOW TO FIX
		//auto window = di->getInstance<AGE::Threads::Render>()->getCommandQueue()->safePriorityFutureEmplace<RendCtxCommand::GetScreenSize, glm::uvec2>().get();

		ImGuiIO& io = ImGui::GetIO();
		//HARDCODED WINDOW TO FIX
		auto screenSize = en->getInstance<IRenderContext>()->getScreenSize();
		io.DisplaySize = ImVec2((float)screenSize.x, (float)screenSize.y);        // Display size, in pixels. For clamping windows positions.
		io.DeltaTime = 1.0f / 60.0f;                          // Time elapsed since last frame, in seconds (in this sample app we'll override this every frame because our timestep is variable)
		io.KeyMap[ImGuiKey_Tab] = AGE_TAB;             // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
		io.KeyMap[ImGuiKey_LeftArrow] = AGE_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = AGE_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = AGE_UP;
		io.KeyMap[ImGuiKey_DownArrow] = AGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = AGE_HOME;
		io.KeyMap[ImGuiKey_End] = AGE_END;
		io.KeyMap[ImGuiKey_Delete] = AGE_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = AGE_BACKSPACE;
		io.KeyMap[ImGuiKey_Enter] = AGE_RETURN;
		io.KeyMap[ImGuiKey_Escape] = AGE_ESCAPE;
		io.KeyMap[ImGuiKey_A] = AGE_a;
		io.KeyMap[ImGuiKey_C] = AGE_c;
		io.KeyMap[ImGuiKey_V] = AGE_v;
		io.KeyMap[ImGuiKey_X] = AGE_x;
		io.KeyMap[ImGuiKey_Y] = AGE_y;
		io.KeyMap[ImGuiKey_Z] = AGE_z;

		io.RenderDrawListsFn = renderDrawLists;

		//io.SetClipboardTextFn = ImImpl_SetClipboardTextFn;
		//io.GetClipboardTextFn = ImImpl_GetClipboardTextFn;

		const GLchar *vertex_shader =
			"#version 330\n"
			"uniform mat4 ProjMtx;\n"
			"in vec2 Position;\n"
			"in vec2 UV;\n"
			"in vec4 Color;\n"
			"out vec2 Frag_UV;\n"
			"out vec4 Frag_Color;\n"
			"void main()\n"
			"{\n"
			"	Frag_UV = UV;\n"
			"	Frag_Color = Color;\n"
			"	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
			"}\n";

		const GLchar* fragment_shader =
			"#version 330\n"
			"uniform sampler2D Texture;\n"
			"in vec2 Frag_UV;\n"
			"in vec4 Frag_Color;\n"
			"out vec4 Out_Color;\n"
			"void main()\n"
			"{\n"
			"	Out_Color = Frag_Color * texture( Texture, Frag_UV.st);\n"
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
		ortho_location = glGetUniformLocation(shader_handle, "ProjMtx");
		position_location = glGetAttribLocation(shader_handle, "Position");
		uv_location = glGetAttribLocation(shader_handle, "UV");
		colour_location = glGetAttribLocation(shader_handle, "Color");

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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		unsigned char* pixels;
		int width, height;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
		io.Fonts->TexID = &fontTex;

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		// Cleanup (don't clear the input data if you want to append new fonts later)
		io.Fonts->ClearInputData();
		io.Fonts->ClearTexData();
#else
		UNUSED(di);
#endif //AGE_ENABLE_IMGUI
		return true;
	}

	Imgui::Imgui()
	{
	}

	void Imgui::startUpdate()
	{
		SCOPE_profile_cpu_function("Main thread");

#ifdef AGE_ENABLE_IMGUI
		ImGuiIO& io = ImGui::GetIO();

		auto input = this->_engine->getInstance<AGE::Input>();

		// Setup timestep
		static Timer timer;
		timer.update();
		float elapsedTime = timer.getElapsed();

		io.DeltaTime = elapsedTime == 0.0 ? 0.000000001f : elapsedTime;
		io.MouseDown[0] = _lastMouseState.mouseState[0];
		io.MouseDown[1] = _lastMouseState.mouseState[1];
		io.MouseDown[2] = _lastMouseState.mouseState[2];
		io.MousePos = ImVec2((float)_lastMouseState.mousePosition.x, (float)_lastMouseState.mousePosition.y);
		io.MouseWheel = _lastMouseState.mouseWheel;
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
#ifdef AGE_ENABLE_IMGUI
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

	void Imgui::renderDrawLists(ImDrawData* draw_data/*ImDrawList** const cmd_lists, int cmd_lists_count*/)
	{
#ifdef AGE_ENABLE_IMGUI
		AGE::GetRenderThread()->setImguiDrawList(std::make_shared<AGE::RenderImgui>(draw_data->CmdLists, draw_data->CmdListsCount));
#else
		UNUSED(cmd_lists);
		UNUSED(cmd_lists_count);
#endif
	}

	void Imgui::renderThreadRenderFn(std::vector<Age_ImDrawList> const &cmd_lists)
	{
		SCOPE_profile_cpu_function("RenderTimer");
		SCOPE_profile_gpu_i("Render IMGUI");
		if (cmd_lists.empty())
			return;
		OpenGLState::glEnable(GL_BLEND);
		OpenGLState::glBlendEquation(GL_FUNC_ADD);
		OpenGLState::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		OpenGLState::glDisable(GL_CULL_FACE);
		OpenGLState::glDisable(GL_DEPTH_TEST);
		OpenGLState::glEnable(GL_SCISSOR_TEST);
		glActiveTexture(GL_TEXTURE0);

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
		glBindVertexArray(vao_handle);

		for (int n = 0; n < cmd_lists.size(); n++)
		{
			const Age_ImDrawList& cmd_list = cmd_lists[n];
			const ImDrawIdx* idx_buffer = cmd_list.idx_buffer.data();

			glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
			size_t needed_vtx_size = cmd_list.vtx_buffer.size() * sizeof(ImDrawVert);
			if (vbo_size < needed_vtx_size)
			{
				// Grow our buffer if needed
				vbo_size = needed_vtx_size + 2000 * sizeof(ImDrawVert);
				glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)vbo_size, NULL, GL_STREAM_DRAW);
			}

			unsigned char* vtx_data = (unsigned char*)glMapBufferRange(GL_ARRAY_BUFFER, 0, needed_vtx_size, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
			if (!vtx_data)
				continue;
			memcpy(vtx_data, &cmd_list.vtx_buffer[0], cmd_list.vtx_buffer.size() * sizeof(ImDrawVert));
			glUnmapBuffer(GL_ARRAY_BUFFER);

			for (auto pcmd = cmd_list.commands.begin(); pcmd != cmd_list.commands.end(); pcmd++)
			{
				glBindTexture(GL_TEXTURE_2D, fontTex);
				glScissor((int)pcmd->ClipRect.x, (int)(height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
				glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, GL_UNSIGNED_SHORT, idx_buffer);
				idx_buffer += pcmd->ElemCount;
			}
		}

		////// Grow our buffer according to what we need
		////size_t total_vtx_count = 0;
		////for (int n = 0; n < cmd_lists.size(); n++)
		////	total_vtx_count += cmd_lists[n].vtx_buffer.size();
		////glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
		////size_t neededBufferSize = total_vtx_count * sizeof(ImDrawVert);
		////if (neededBufferSize > vbo_max_size)
		////{
		////	vbo_max_size = neededBufferSize + 5000;  // Grow buffer
		////	glBufferData(GL_ARRAY_BUFFER, vbo_max_size, NULL, GL_STREAM_DRAW);
		////}

		////// Copy and convert all vertices into a single contiguous buffer
		////unsigned char* buffer_data = (unsigned char*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		////if (!buffer_data)
		////	return;
		////for (int n = 0; n < cmd_lists.size(); n++)
		////{
		////	auto &cmd_list = cmd_lists[n];
		////	memcpy(buffer_data, &cmd_list.vtx_buffer[0], cmd_list.vtx_buffer.size() * sizeof(ImDrawVert));
		////	buffer_data += cmd_list.vtx_buffer.size() * sizeof(ImDrawVert);
		////}
		////glUnmapBuffer(GL_ARRAY_BUFFER);
		////glBindBuffer(GL_ARRAY_BUFFER, 0);
		////glBindVertexArray(vao_handle);

		////int cmd_offset = 0;
		////for (int n = 0; n < cmd_lists.size(); n++)
		////{
		////	SCOPE_profile_cpu_i("RenderTimer", "Render ImGui command list");
		////	auto &cmd_list = cmd_lists[n];
		////	int vtx_offset = cmd_offset;
		////	auto &pcmd_end = std::end(cmd_list.commands);
		////	for (auto &pcmd = std::begin(cmd_list.commands); pcmd != pcmd_end; pcmd++)
		////	{
		////		glScissor((int)pcmd->ClipRect.x, (int)(height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
		////		//glDrawElements(GL_TRIANGLES, pcmd->ElemCount, GL_UNSIGNED_SHORT, );
		////		vtx_offset += pcmd->vtx_count;
		////	}
		////	cmd_offset = vtx_offset;
		////}

		// Restore modified state
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glUseProgram(0);
		OpenGLState::glDisable(GL_SCISSOR_TEST);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Imgui::initShader(int *pid, int *vert, int *frag, const char *vs, const char *fs)
	{
#ifdef AGE_ENABLE_IMGUI
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