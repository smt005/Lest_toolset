
#include "GLFW/glfw3.h"
#include "Editor/ObjectEditor.h"
#include "Editor/ImGuiManage.h"
#include "Help/Help.h"

#ifdef _DEBUG
	#include "../Build/git_hash.h"
	#include "../Build/git_name_branch.h"
	#define APP_INFO " [ " __DATE__"  " __TIME__" ] [ " GIT_NAME_BRANCH  " - " GIT_HASH " ]"
#endif

#if WINDOWS_HIDE_CONSOLE
#include <Windows.h>
#endif

namespace {
	const char* title = "Test Lesta tools";
	int width = 800;
	int height = 600;
	int minWidth = 800;
	int minHeight = 600;
	int maxWidth = 2560;
	int maxHeight = 1440;

	std::unique_ptr<Window::ObjectEditor> editorPtr;

	void WindowSizeCallback(GLFWwindow* window, int width, int height) {
		editorPtr->Resize(static_cast<float>(width), static_cast<float>(height));
	}

	void Terminate() {
		editorPtr.reset();
		Window::Cleanup();
		glfwTerminate();
	}

	int Runwindow() {
#if WINDOWS_HIDE_CONSOLE
		FreeConsole();
#endif

		if (!glfwInit()) {
			return 1;
		}

#ifdef _DEBUG
		std::string titleEndInfo = std::string(title) + APP_INFO;
		GLFWwindow* window = glfwCreateWindow(width, height, titleEndInfo.c_str(), nullptr, nullptr);
#else
		GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
#endif
		if (!window) {
			glfwTerminate();
			return 1;
		}

		glfwMakeContextCurrent(window);
		glfwSetWindowSizeCallback(window, WindowSizeCallback);
		glfwSetWindowSizeLimits(window, minWidth, minHeight, maxWidth, maxHeight);

		Window::Init(window);

		// Первый аргумен определяет в какой папке ресурсы относительно приложения.
		// Второй параметр определяет название файла с даными.
		editorPtr = std::make_unique<Window::ObjectEditor>("/../Resources", "Objects.json");
		if (!editorPtr) {
			Terminate();
			return 1;
		}	
		Window::RenderPush();
		editorPtr->InitDisplay(static_cast<float>(width), static_cast<float>(height));
		Window::RenderPop();

		editorPtr->Load();

		while (!glfwWindowShouldClose(window)) {
			Window::RenderPush();			
			editorPtr->Render();
			Window::RenderPop();

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		Terminate();
		return 0;
	}
}

int main(int argc, char** argv) {
	return Runwindow();
}
