
#include "PopupModal.h"
#include "imgui.h"

namespace Window {
	std::weak_ptr<bool> PopupModal::_weakWndPtr;
	std::function<void(void)> PopupModal::_content = nullptr;
	std::string PopupModal::_titlePopupModal;
	bool PopupModal::_statePopupModal = false;

	void PopupModal::Show(const std::weak_ptr<bool>& weakWndPtr, const std::function<void(void)>& content, const std::string& titlePopupModal) {
		_weakWndPtr = weakWndPtr;
		_content = content;
		_titlePopupModal = titlePopupModal;
		_statePopupModal = true;
	}

	void PopupModal::Hide() {
		ImGui::CloseCurrentPopup();

		_content = nullptr;
		_weakWndPtr.reset();
		_titlePopupModal.clear();
	}

	void PopupModal::Draw() {
		if (!_content) {
			return;
		}

		if (_weakWndPtr.expired()) {
			Hide();
			return;
		}

		if (_statePopupModal) {
			ImGui::OpenPopup(_titlePopupModal.c_str());
			_statePopupModal = false;
		}

		if (ImGui::BeginPopupModal(_titlePopupModal.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
			_content();
			ImGui::EndPopup();
		}
	}
}
