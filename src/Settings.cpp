#include "Settings.h"

namespace LoadingScreenLocker {

Settings *Settings::GetSingleton() {
  static Settings singleton;
  return &singleton;
}

void Settings::Load() {
  constexpr auto path = L"Data/SKSE/Plugins/LoadingScreenLocker.ini";

  CSimpleIniA ini;
  ini.SetUnicode();

  SI_Error rc = ini.LoadFile(path);
  if (rc < 0) {
    logger::info("No INI file found at {}, using defaults",
                 "Data/SKSE/Plugins/LoadingScreenLocker.ini");
    return;
  }

  // General
  bEnable = ini.GetBoolValue("General", "bEnable", bEnable);

  // Filtering
  bDisableMouseMove =
      ini.GetBoolValue("Filtering", "bDisableMouseMove", bDisableMouseMove);
  bDisableThumbsticks =
      ini.GetBoolValue("Filtering", "bDisableThumbsticks", bDisableThumbsticks);

  // UserEventGate
  bUserEventGate =
      ini.GetBoolValue("UserEventGate", "bUserEventGate", bUserEventGate);

  const char *allowlistStr =
      ini.GetValue("UserEventGate", "sUserEventAllowlist", "rotate,look");
  if (allowlistStr && *allowlistStr) {
    userEventAllowlist.clear();
    std::string_view sv(allowlistStr);
    size_t pos = 0;
    while (pos < sv.size()) {
      size_t comma = sv.find(',', pos);
      if (comma == std::string_view::npos) {
        comma = sv.size();
      }
      auto token = sv.substr(pos, comma - pos);
      // Trim whitespace
      while (!token.empty() && token.front() == ' ')
        token.remove_prefix(1);
      while (!token.empty() && token.back() == ' ')
        token.remove_suffix(1);
      if (!token.empty()) {
        userEventAllowlist.emplace_back(token);
      }
      pos = comma + 1;
    }
  }

  logger::info("Settings loaded:");
  logger::info("  bEnable = {}", bEnable);
  logger::info("  bDisableMouseMove = {}", bDisableMouseMove);
  logger::info("  bDisableThumbsticks = {}", bDisableThumbsticks);
  logger::info("  bUserEventGate = {}", bUserEventGate);
  logger::info("  Allowlist entries: {}", userEventAllowlist.size());
}

} // namespace LoadingScreenLocker
