#include "InputHandler.h"
#include "Settings.h"

namespace LoadingScreenLocker {

InputHandler *InputHandler::GetSingleton() {
  static InputHandler singleton;
  return &singleton;
}

bool InputHandler::IsLoadingMenuActive() {
  auto *ui = RE::UI::GetSingleton();
  return ui && ui->IsMenuOpen(RE::LoadingMenu::MENU_NAME);
}

bool InputHandler::ShouldFilterUserEvent(
    const RE::BSFixedString &userEvent) const {
  auto *settings = Settings::GetSingleton();

  // If user event gating is disabled, filter all matching input types
  if (!settings->bUserEventGate) {
    return true;
  }

  // No user event associated - filter it (conservative approach)
  if (userEvent.empty()) {
    return true;
  }

  // Check if the user event is in the allowlist
  std::string_view eventView(userEvent.c_str());
  for (const auto &allowed : settings->userEventAllowlist) {
    // Case-insensitive comparison
    if (eventView.size() == allowed.size()) {
      bool match = true;
      for (size_t i = 0; i < eventView.size() && match; ++i) {
        match = (std::tolower(static_cast<unsigned char>(eventView[i])) ==
                 std::tolower(static_cast<unsigned char>(allowed[i])));
      }
      if (match) {
        return true;
      }
    }
  }

  // User event not in allowlist - do not filter (let it through)
  return false;
}

void InputHandler::Install() {
  // Install the MenuControls::ProcessEvent hook
  MenuControlsHook::Install();
  logger::info("Input filtering hooks installed");
}

// MenuControlsHook implementation
RE::BSEventNotifyControl MenuControlsHook::ProcessEvent_Hook(
    RE::MenuControls *a_this, RE::InputEvent *const *a_event,
    RE::BSTEventSource<RE::InputEvent *> *a_eventSource) {

  auto *handler = InputHandler::GetSingleton();
  auto *settings = Settings::GetSingleton();

  // Only filter if enabled and loading menu is active
  if (settings->bEnable && handler->IsLoadingMenuActive() && a_event) {
    // Iterate through the event chain and zero rotation/zoom inputs
    for (auto *event = *a_event; event; event = event->next) {
      if (!event) {
        continue;
      }

      const auto eventType = event->GetEventType();

      // Mouse movement filtering
      if (settings->bDisableMouseMove &&
          eventType == RE::INPUT_EVENT_TYPE::kMouseMove) {
        auto *mouseMove = static_cast<RE::MouseMoveEvent *>(event);
        const auto &userEvent = event->QUserEvent();
        if (handler->ShouldFilterUserEvent(userEvent)) {
          mouseMove->mouseInputX = 0;
          mouseMove->mouseInputY = 0;
        }
      }

      // Thumbstick filtering (both sticks)
      if (settings->bDisableThumbsticks &&
          eventType == RE::INPUT_EVENT_TYPE::kThumbstick) {
        auto *thumbstick = static_cast<RE::ThumbstickEvent *>(event);
        const auto &userEvent = event->QUserEvent();
        if (handler->ShouldFilterUserEvent(userEvent)) {
          thumbstick->xValue = 0.0f;
          thumbstick->yValue = 0.0f;
        }
      }
    }
  }

  // Call the original function
  return _ProcessEvent(a_this, a_event, a_eventSource);
}

void MenuControlsHook::Install() {
  // Hook MenuControls::ProcessEvent (virtual function index 1 for InputEvent
  // sink) BSTEventSink<InputEvent*> is the first parent class, so vtable[0] is
  // at index 0 Index 0 = destructor, Index 1 = ProcessEvent
  REL::Relocation<std::uintptr_t> vtable(RE::VTABLE_MenuControls[0]);

  _ProcessEvent = vtable.write_vfunc(0x1, ProcessEvent_Hook);

  logger::info("MenuControls::ProcessEvent hooked");
}

} // namespace LoadingScreenLocker
