#include <Xinput.h>

typedef struct {
	WORD buttons;			// Bitmask of pressed buttons
	float leftThumb[2];		// [-1.0f, 1.0f]
    float rightThumb[2];	// [-1.0f, 1.0f]
    bool leftBackTrigger;	// ON / OFF
	bool rightBackTrigger;	// ON / OFF
} ControllerInput;

class Controller {
private:
	DWORD controllerIndex;
	XINPUT_STATE state;
	XINPUT_STATE previousState;

	ControllerInput input;

public:
	Controller(DWORD controllerIndex) : controllerIndex(controllerIndex), state{}, previousState{}, input{} {}
	~Controller() = default;

	bool IsConnected();
	bool HasUpdatedState() const;
	ControllerInput& GetInput() { return this->input; }

	ControllerInput& ReadNewInput();
};