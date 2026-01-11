#pragma once

#include "PCH.h"

namespace LoadingScreenLocker {

/**
 * Manages plugin settings loaded from INI file.
 */
class Settings {
public:
  static Settings *GetSingleton();

  void Load();

  // Master enable
  bool bEnable = true;

  // Filtering options
  bool bDisableMouseMove = true;
  bool bDisableThumbsticks = true;

  // User event gating
  bool bUserEventGate = true;
  std::vector<std::string> userEventAllowlist = {"rotate", "look"};

private:
  Settings() = default;
  Settings(const Settings &) = delete;
  Settings &operator=(const Settings &) = delete;
};

} // namespace LoadingScreenLocker
