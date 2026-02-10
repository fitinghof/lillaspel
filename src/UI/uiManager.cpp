#include "UI/uiManager.h"

#include <algorithm>

namespace UI {
UIManager* uiManager = nullptr;
}

void UI::UIManager::AddCanvas(const std::shared_ptr<Canvas>& canvas) {
	if (!canvas) return;
	if (std::find(this->canvases.begin(), this->canvases.end(), canvas) != this->canvases.end()) return;
	this->canvases.push_back(canvas);
}

void UI::UIManager::RemoveCanvas(const std::shared_ptr<Canvas>& canvas) {
	this->canvases.erase(std::remove(this->canvases.begin(), this->canvases.end(), canvas), this->canvases.end());
	this->hoveredWidget.reset();
	this->pressedWidget.reset();
}

void UI::UIManager::ClearCanvases() {
	this->canvases.clear();
	this->hoveredWidget.reset();
	this->pressedWidget.reset();
}

const std::vector<std::shared_ptr<UI::Canvas>>& UI::UIManager::GetCanvases() const { return this->canvases; }

void UI::UIManager::Update(float dt) {
	for (auto& canvas : this->canvases) {
		if (canvas) canvas->Update(dt);
	}
}

void UI::UIManager::Draw() {
	for (auto& canvas : this->canvases) {
		if (canvas) canvas->Draw();
	}
}

void UI::UIManager::OnMouseMove(Vec2 pos) {
	auto newHover = this->HitTestAll(pos);
	if (newHover != this->hoveredWidget) this->HandleHoverChange(newHover);
}

void UI::UIManager::OnMouseDown(int button, Vec2 pos) {
	(void) button;
	this->HandlePress(this->HitTestAll(pos));
}

void UI::UIManager::OnMouseUp(int button, Vec2 pos) {
	(void) button;
	this->HandleRelease(this->HitTestAll(pos));
}

std::shared_ptr<UI::Widget> UI::UIManager::HitTestAll(Vec2 pos) const {
	for (auto it = this->canvases.rbegin(); it != this->canvases.rend(); ++it) {
		if (!*it) continue;
		auto hit = (*it)->HitTest(pos);
		if (hit) return hit;
	}

	return nullptr;
}

void UI::UIManager::HandleHoverChange(const std::shared_ptr<Widget>& newHover) { this->hoveredWidget = newHover; }

void UI::UIManager::HandlePress(const std::shared_ptr<Widget>& target) { this->pressedWidget = target; }

void UI::UIManager::HandleRelease(const std::shared_ptr<Widget>& target) {
	(void) target;
	this->pressedWidget.reset();
}
