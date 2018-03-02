///  by Carl Findahl (C) 2018
/// A Kukon Development Project

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "linalg.h"

#include <array>
#include <type_traits>


class InputManager {
public:
	InputManager() { mInputMask.fill(0); }

	// Clear the pressed key input mask
	void clear() { mPressedKeyMask.fill(0); }

    // Tell input manager that the wheel was scrolled with the given delta
    void scrollMouse(const vec2d& delta) { mScrollDelta = delta; }

    // Get the latest scroll delta
    const vec2d& getScrollDelta() const { return mScrollDelta; }

    // Tell input manager that the cursor has moved
    void moveCursor(const vec2d& newPosition) { mCursorPosition = newPosition; }

    // Get the latest polled cursor position
    const vec2d& getCursorPosition() const { return mCursorPosition; }

	// Tell input manager that the given key / mouse button has been pressed
	void pressKey(unsigned key) { ++mInputMask[key]; mPressedKeyMask[key] = 1u; }

	// Tell input manager that the given key / mouse button has been released
	void releaseKey(unsigned key) { mInputMask[key] = 0; mPressedKeyMask[key] = 0; }

	// Check if all given keys are currently in a pressed state or repeating [live]
    // This supports keys such as Ctrl, Alt Shift and all Mouse buttons too
	template<typename... Keys>
	const bool arePressed(Keys... keys) const { return (... && mInputMask[keys]); }

	// Check if all given keys were pressed down this frame [event-based]
    // This supports keys such as Ctrl, Alt Shift and all Mouse buttons too
	template<typename... Keys>
	const bool wasPressed(Keys... keys) const { return (... && (mPressedKeyMask[keys] == 1)); }

private:
	// Input Mask for keyboard keys and mouse buttons [live]
	std::array<unsigned, 512> mInputMask;

	// Input Mask for keyboard keys and mouse buttons [event based]
	std::array<unsigned, 512> mPressedKeyMask;

    // The current position of the cursor
    vec2d mCursorPosition;

    // Stores the latest scroll delta
    vec2d mScrollDelta;
};


#endif // INPUTMANAGER_H
