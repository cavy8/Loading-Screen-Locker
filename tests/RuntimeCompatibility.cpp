#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace {
void Check(bool condition, const char *message) {
  if (!condition) {
    throw std::runtime_error(message);
  }
}

void CheckRuntime(REL::Version version, REL::Module::Runtime runtime,
                  const std::filesystem::path &fixture, std::size_t offset) {
  // Leave runtime selection automatic so 1.7 must select AE, not SE.
  Check(REL::Module::mock(version), "Cannot mock runtime");
  Check(REL::Module::GetRuntime() == runtime, "Wrong runtime family");
  Check(REL::IDDB::inject(fixture.wstring(), version),
        "Cannot auto-detect Address Library format");
  Check(REL::IDDB::get().id2offset(11483) == offset, "Wrong address lookup");
  const REL::VariantID id(runtime == REL::Module::Runtime::SE ? 11483 : 0,
                          runtime == REL::Module::Runtime::AE ? 11483 : 0,
                          0x1234);
  Check(id.offset() == (runtime == REL::Module::Runtime::VR ? 0x1234 : offset),
        "Wrong cross-runtime relocation");
  std::cout << version.string() << " runtime and relocations passed\n";
  REL::IDDB::reset();
  REL::Module::reset();
}
} // namespace

int main(int argc, char **argv) {
  try {
    Check(argc == 2, "Expected CommonLibSSE fixture directory");
    const std::filesystem::path fixtures(argv[1]);
    using Runtime = REL::Module::Runtime;
    CheckRuntime({1, 5, 97, 0}, Runtime::SE,
                 fixtures / "version-1-5-97-0.bin", 0x10f5c0);
    CheckRuntime({1, 6, 353, 0}, Runtime::AE,
                 fixtures / "versionlib-1-6-353-0.bin", 0x10f7a0);
    CheckRuntime({1, 6, 1130, 0}, Runtime::AE,
                 fixtures / "versionlib-1-6-1130-0.bin", 0x14fbe0);
    CheckRuntime({1, 6, 1170, 0}, Runtime::AE,
                 fixtures / "versionlib-1-6-1170-0.bin", 0x14fcd0);
    CheckRuntime({1, 6, 1179, 0}, Runtime::AE,
                 fixtures / "versionlib-1-6-1179-0.bin", 0x14fb00);
    CheckRuntime({1, 7, 99, 0}, Runtime::AE,
                 fixtures / "versionlib-1-7-99-0.bin", 0x155260);

    // Synthetic 1.7.104 fixture: checks format 5 and runtime selection,
    // not the addresses in a real 1.7.104 executable.
    const auto synthetic = std::filesystem::current_path() / "test-1-7-104.bin";
    std::filesystem::copy_file(fixtures / "versionlib-1-7-99-0.bin", synthetic,
                              std::filesystem::copy_options::overwrite_existing);
    {
      std::fstream file(synthetic, std::ios::in | std::ios::out | std::ios::binary);
      Check(file.is_open(), "Cannot open synthetic fixture");
      const std::uint32_t version[] = {1, 7, 104, 0};
      file.seekp(sizeof(std::int32_t));
      file.write(reinterpret_cast<const char *>(version), sizeof(version));
      Check(file.good(), "Cannot write synthetic fixture");
    }
    CheckRuntime({1, 7, 104, 0}, Runtime::AE, synthetic, 0x155260);
    std::filesystem::remove(synthetic);

    Check(REL::Module::mock({1, 4, 15, 0}), "Cannot mock VR");
    Check(REL::Module::IsVR(), "Wrong VR runtime family");
    Check(REL::IDDB::inject((fixtures / "version-1-4-15-0.csv").wstring(),
                           REL::IDDB::Format::VR), "Cannot load VR Address Library");
    Check(RE::VTABLE_MenuControls[0].offset() == 0x173c548,
          "Wrong VR MenuControls vtable relocation");
    std::cout << "1.4.15 VR runtime and vtable passed\n";
    REL::IDDB::reset();
    REL::Module::reset();
    return 0;
  } catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
  }
}
