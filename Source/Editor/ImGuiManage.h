#pragma once

namespace Window {
	void Init(void* window);
	void Cleanup();
	void RenderPush();
	void RenderPop();
}
