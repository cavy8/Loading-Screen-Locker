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
#ifdef NDEBUG
#include <spdlog/sinks/basic_file_sink.h>
#else
#include <spdlog/sinks/msvc_sink.h>
#endif

// ---- Third-party Libraries ----
#include <SimpleIni.h>

// ---- Macros ----
#define DLLEXPORT __declspec(dllexport)

// ---- Logging Helpers ----
namespace logger = SKSE::log;

using namespace std::literals;
