#pragma once

#include "UI/button.h"
#include "UI/canvas.h"

namespace UI {

class UIManager {
public:
	UIManager() = default;

	// Canvas management
	void AddCanvas(const std::shared_ptr<Canvas>& canvas);
	void RemoveCanvas(const std::shared_ptr<Canvas>& canvas);
	void ClearCanvases();
	const std::vector<std::shared_ptr<Canvas>>& GetCanvases() const;

	// Main loop
	void Update(float dt);
	void Draw();

	// Input routing
	void OnMouseMove(Vec2 pos);
	void OnMouseDown(int button, Vec2 pos);
	void OnMouseUp(int button, Vec2 pos);

private:
	std::vector<std::shared_ptr<Canvas>> canvases;

	std::shared_ptr<Widget> hoveredWidget;
	std::shared_ptr<Widget> pressedWidget;

	std::shared_ptr<Widget> HitTestAll(Vec2 pos) const;
	void HandleHoverChange(const std::shared_ptr<Widget>& newHover);
	void HandlePress(const std::shared_ptr<Widget>& target);
	void HandleRelease(const std::shared_ptr<Widget>& target);
};

extern UIManager* uiManager;

} // namespace UI