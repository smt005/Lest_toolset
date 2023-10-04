#include "ImGuiManage.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

namespace Window {
	void Init(void* window) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		// Тема, светлая и тёмная
		//ImGui::StyleColorsDark();
		ImGui::StyleColorsLight();

		ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(window), true);
		ImGui_ImplOpenGL3_Init("#version 130");
	}

	void Cleanup() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void RenderPush() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void RenderPop() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}
