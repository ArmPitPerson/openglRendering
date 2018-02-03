///  by Carl Findahl (C) 2018
/// A Kukon Development Project

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <array>


class InputManager {
public:
	InputManager() { mInputMask.fill(0); }

	// Clear pressed input mask
	void clear() { mPressedKeyMask.fill(0); }

	// Set a key to have been pressed
	void triggerKey(unsigned key) { ++mInputMask[key]; mPressedKeyMask[key] = 1u; }

	// Set a key to have been released
	void unTriggerKey(unsigned key) { mInputMask[key] = 0; mPressedKeyMask[key] = 0; }

	// Check if a key is being repeated
	const bool isKeyRepeating(unsigned key) const { return mInputMask[key] > 1; }

	// Check if all given keys are pressed down
	template<typename... Keys>
	const bool areKeysDown(Keys... keys) const { return (... && mInputMask[keys]); }

	// Check if a key was pressed for the first time
	const bool wasPressed(unsigned key) const { return mPressedKeyMask[key] == 1; }

private:
	// Input Mask for keyboard keys and mouse buttons [live]
	std::array<unsigned, 512> mInputMask;

	// Input Mask for keyboard keys and mouse buttons [pressed]
	std::array<unsigned, 512> mPressedKeyMask;
};


#endif // INPUTMANAGER_H
