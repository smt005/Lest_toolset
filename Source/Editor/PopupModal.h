#pragma once

#include <functional>
#include <memory>
#include <string>

namespace Window {
	class PopupModal {
	public:
	public:
		static void Show(const std::weak_ptr<bool>& weakWndPtr, const std::function<void(void)>& content, const std::string& titlePopupModal = "Window");
		static void Hide();
		static void Draw();

	private:
		static std::weak_ptr<bool>			_weakWndPtr;
		static std::function<void(void)>	_content;
		static std::string					_titlePopupModal;
		static bool							_statePopupModal;
	};
}