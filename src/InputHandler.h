#pragma once

#include "PCH.h"

namespace LoadingScreenLocker {

/**
 * Input handler singleton that manages filtering logic.
 */
class InputHandler {
public:
  static InputHandler *GetSingleton();

  void Install();

  bool IsLoadingMenuActive();
  bool ShouldFilterUserEvent(const RE::BSFixedString &userEvent) const;

private:
  InputHandler() = default;
  InputHandler(const InputHandler &) = delete;
  InputHandler &operator=(const InputHandler &) = delete;
};

/**
 * Hook for MenuControls::ProcessEvent to intercept input during loading
 * screens.
 */
struct MenuControlsHook {
  // Hook function - must be a thiscall for vtable replacement
  static RE::BSEventNotifyControl
  ProcessEvent_Hook(RE::MenuControls *a_this, RE::InputEvent *const *a_event,
                    RE::BSTEventSource<RE::InputEvent *> *a_eventSource);

  static void Install();

  // Original function pointer
  static inline REL::Relocation<decltype(&ProcessEvent_Hook)> _ProcessEvent;
};

} // namespace LoadingScreenLocker
