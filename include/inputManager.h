///  by Carl Findahl (C) 2018
/// A Kukon Development Project

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <array>
#include <type_traits>


class InputManager {
public:
	InputManager() { mInputMask.fill(0); }

	// Clear pressed input mask
	void clear() { mPressedKeyMask.fill(0); }

	// Tell input manager that the given key has been pressed
	void pressKey(unsigned key) { ++mInputMask[key]; mPressedKeyMask[key] = 1u; }

	// Tell input manager that the given key has been released
	void releaseKey(unsigned key) { mInputMask[key] = 0; mPressedKeyMask[key] = 0; }

	// Check if all given keys are currently in a pressed state or repeating [live]
	template<typename... Keys>
	const bool arePressed(Keys... keys) const { return (... && mInputMask[keys]); }

	// Check if all given keys were pressed down this frame [event-based]
	template<typename... Keys>
	const bool wasPressed(Keys... keys) const { return (... && (mPressedKeyMask[keys] == 1)); }

private:
	// Input Mask for keyboard keys and mouse buttons [live]
	std::array<unsigned, 512> mInputMask;

	// Input Mask for keyboard keys and mouse buttons [pressed]
	std::array<unsigned, 512> mPressedKeyMask;
};


#endif // INPUTMANAGER_H
