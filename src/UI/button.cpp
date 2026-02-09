#include "button.h"

#include "core/input/inputManager.h"

UI::Button::Button(MeshObjData& mesh) : Widget(mesh) {}

void UI::Button::Update(float dt) {
	if (!this->visible || !this->enabled) return;

	auto [mx, my] = InputManager::GetInstance().GetMouseMovement();
	Vec2 point{static_cast<float>(mx), static_cast<float>(my)};
	const bool hit = this->HitTest(point);

	if (hit && !this->hovered) {
		this->hovered = true;
		if (this->onHover) this->onHover();
	} else if (!hit && this->hovered) {
		this->hovered = false;
		if (this->onUnhover) this->onUnhover();
	}

	if (hit && InputManager::GetInstance().WasLMPressed()) {
		this->pressed = true;
		if (this->onPressed) this->onPressed();
	}

	if (hit && InputManager::GetInstance().WasLMReleased()) {
		this->pressed = false;
		if (this->onReleased) this->onReleased();
		if (hit && this->onClick) this->onClick();
	}

	Widget::Update(dt);
}

void UI::Button::SetOnClick(std::function<void()> callback) { this->onClick = std::move(callback); }

void UI::Button::SetOnPressed(std::function<void()> callback) { this->onPressed = std::move(callback); }

void UI::Button::SetOnReleased(std::function<void()> callback) { this->onReleased = std::move(callback); }

void UI::Button::SetOnHover(std::function<void()> callback) { this->onHover = std::move(callback); }

void UI::Button::SetOnUnhover(std::function<void()> callback) { this->onUnhover = std::move(callback); }

bool UI::Button::IsHovered() const { return this->hovered; }

bool UI::Button::isPressed() const { return this->pressed; }
