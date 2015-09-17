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
		ImGuiKeyEvent(AgeKeys _key, bool _down);
	};

	struct ImGuiEndOfFrame
	{
	};
	
	struct ImGuiMouseStateEvent
	{
		glm::ivec2 mousePosition;
		bool mouseState[3];
		float mouseWheel;

		ImGuiMouseStateEvent();
		ImGuiMouseStateEvent(glm::ivec2 const &_mousePosition, bool leftClic, bool wheelClic, bool rightClic, float _mouseWheel);
	};

	struct Age_ImDrawList
	{
		std::vector<ImDrawCmd>     commands;
		std::vector<ImDrawVert>    vtx_buffer;
		std::vector<ImDrawIdx>     idx_buffer;
		Age_ImDrawList() = default;
		Age_ImDrawList(const Age_ImDrawList& o);
		Age_ImDrawList& operator=(const Age_ImDrawList& o);
		Age_ImDrawList(const ImDrawList& o);
	};

	struct RenderImgui
	{
		std::vector<Age_ImDrawList> cmd_lists;
		RenderImgui(ImDrawList** const _cmd_lists, int _cmd_lists_count);
		RenderImgui(const RenderImgui &o);
		RenderImgui &operator=(const RenderImgui &o);
	};


	class Imgui
	{
		static int shader_handle, vert_handle, frag_handle;
		static int texture_location, ortho_location;
		static int position_location, uv_location, colour_location;
		static size_t vbo_max_size, vbo_size;
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
		static void renderDrawLists(ImDrawData* draw_data);
		static void initShader(int *pid, int *vert, int *frag, const char *vs, const char *fs);
		void renderThreadRenderFn(const std::vector<Age_ImDrawList> &cmd_lists);
	};
}