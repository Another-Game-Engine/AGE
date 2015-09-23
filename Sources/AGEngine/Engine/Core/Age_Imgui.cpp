#include "Age_Imgui.hpp"
#include <Context/IRenderContext.hh>

#include <SDL/SDL_keycode.h>

#include <imgui/imconfig.h>

#include <Utils/Utils.hh>
#include <Utils/Profiler.hpp>

#include <Threads/MainThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Threads/RenderThread.hpp>

#include <Core/Engine.hh>
#include <Core/Inputs/Input.hh>
#include <Core/Timer.hh>

#include <Render/OpenGLTask/OpenGLState.hh>

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

	ImGuiKeyEvent::ImGuiKeyEvent(AgeKeys _key, bool _down)
		: key(_key)
		, down(_down)
	{}

	ImGuiMouseStateEvent::ImGuiMouseStateEvent() :
		mousePosition(0)
		, mouseWheel(0.0f)
	{
		mouseState[0] = false;
		mouseState[1] = false;
		mouseState[2] = false;
	}

	ImGuiMouseStateEvent::ImGuiMouseStateEvent(glm::ivec2 const &_mousePosition, bool leftClic, bool wheelClic, bool rightClic, float _mouseWheel)
		: mousePosition(_mousePosition)
		, mouseWheel(_mouseWheel)
	{
		mouseState[0] = leftClic;
		mouseState[1] = wheelClic;
		mouseState[2] = rightClic;
	}

	Age_ImDrawList::Age_ImDrawList(const Age_ImDrawList& o)
	{
		commands = o.commands;
		vtx_buffer = o.vtx_buffer;
		idx_buffer = o.idx_buffer;
	}

	Age_ImDrawList& Age_ImDrawList::operator=(const Age_ImDrawList& o)
	{
		commands = o.commands;
		vtx_buffer = o.vtx_buffer;
		idx_buffer = o.idx_buffer;
		return *this;
	}

	Age_ImDrawList::Age_ImDrawList(const ImDrawList& o)
	{
		const ImDrawCmd* pcmd_end = o.CmdBuffer.end();
		commands.resize(o.CmdBuffer.size());
		int i = 0;
		for (const ImDrawCmd* pcmd = o.CmdBuffer.begin(); pcmd != pcmd_end; pcmd++)
		{
			commands[i] = (*pcmd);
			++i;
		}
		auto ve = o.VtxBuffer.end();
		vtx_buffer.resize(o.VtxBuffer.size());
		i = 0;
		for (auto v = o.VtxBuffer.begin(); v != ve; v++)
		{
			vtx_buffer[i] = *v;
			++i;
		}
		auto vi = o.IdxBuffer.end();
		idx_buffer.resize(o.IdxBuffer.size());
		i = 0;
		for (auto v = o.IdxBuffer.begin(); v != vi; v++)
		{
			idx_buffer[i] = *v;
			++i;
		}
	}

	RenderImgui::RenderImgui(ImDrawList** const _cmd_lists, int _cmd_lists_count)
	{
		cmd_lists.resize(_cmd_lists_count);
		for (auto i = 0; i < _cmd_lists_count; ++i)
			cmd_lists[i] = Age_ImDrawList(*_cmd_lists[i]);
	}

	RenderImgui::RenderImgui(const RenderImgui &o)
	{
		cmd_lists = o.cmd_lists;
	}

	RenderImgui &RenderImgui::operator=(const RenderImgui &o)
	{
		cmd_lists = o.cmd_lists;
		return *this;
	}



	bool Imgui::init(Engine *en)
	{
#ifdef AGE_ENABLE_IMGUI

		GetMainThread()->registerCallback<ImGuiKeyEvent>([this](ImGuiKeyEvent &msg)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.KeysDown[int(msg.key)] = msg.down;
			if (msg.key == AgeKeys::AGE_LCTRL || msg.key == AgeKeys::AGE_RCTRL)
				io.KeyCtrl = msg.down;
			else if (msg.key == AgeKeys::AGE_LSHIFT || msg.key == AgeKeys::AGE_RSHIFT)
				io.KeyShift = msg.down;
			else if (msg.down)
			{
				char character = keyToAscii.find(msg.key)->second;
				if (character != UNDEFINED_CHARACTER)
					io.AddInputCharacter(character);
			}
		});

		GetMainThread()->registerCallback<ImGuiEndOfFrame>([this](ImGuiEndOfFrame &msg)
		{
			SCOPE_profile_cpu_i("ImGui", "Render");
			ImGui::Render();
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
		io.KeyMap[ImGuiKey_Tab] = int(AgeKeys::AGE_TAB);             // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
		io.KeyMap[ImGuiKey_LeftArrow] =  int(AgeKeys::AGE_LEFT);
		io.KeyMap[ImGuiKey_RightArrow] = int(AgeKeys::AGE_RIGHT);
		io.KeyMap[ImGuiKey_UpArrow] =    int(AgeKeys::AGE_UP);
		io.KeyMap[ImGuiKey_DownArrow] =  int(AgeKeys::AGE_DOWN);
		io.KeyMap[ImGuiKey_Home] =       int(AgeKeys::AGE_HOME);
		io.KeyMap[ImGuiKey_End] =        int(AgeKeys::AGE_END);
		io.KeyMap[ImGuiKey_Delete] =     int(AgeKeys::AGE_DELETE);
		io.KeyMap[ImGuiKey_Backspace] =  int(AgeKeys::AGE_BACKSPACE);
		io.KeyMap[ImGuiKey_Enter] =      int(AgeKeys::AGE_RETURN);
		io.KeyMap[ImGuiKey_Escape] =     int(AgeKeys::AGE_ESCAPE);
		io.KeyMap[ImGuiKey_A] =          int(AgeKeys::AGE_a);
		io.KeyMap[ImGuiKey_C] =          int(AgeKeys::AGE_c);
		io.KeyMap[ImGuiKey_V] =          int(AgeKeys::AGE_v);
		io.KeyMap[ImGuiKey_X] =          int(AgeKeys::AGE_x);
		io.KeyMap[ImGuiKey_Y] =          int(AgeKeys::AGE_y);
		io.KeyMap[ImGuiKey_Z] =          int(AgeKeys::AGE_z);

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