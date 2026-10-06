#pragma once

// ---- CommonLib NG ----
#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>

// ---- Standard Library ----
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

// ---- SKSE Versioning ----
#include <spdlog/sinks/basic_file_sink.h>

// ---- Third-party Libraries ----
#include <SimpleIni.h>

// ---- Macros ----
#define DLLEXPORT __declspec(dllexport)

// ---- Logging Helpers ----
namespace logger = SKSE::log;

using namespace std::literals;
