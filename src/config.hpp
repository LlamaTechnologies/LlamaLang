#pragma once
#include <string>
#include <vector>

enum class MACHINE_ARCH {
    AMD_X64,
    ARM_X64,
    PS4,
    PS5,
    XBOX_ONE,
    XBOX_SERIES,
};

enum class TARGET_OS {
    WINDOWS,
    LINUX,
    MACOS,
    ANDROID,
    IOS,
    PS4,
    PS5,
    XBOX_ONE,
    XBOX_SERIES,
};

enum class OPT_TARGET {
    NONE,
    SIZE,
    PERFORMANCE,
    BALANCED,
};

enum class BUILD_TARGET {
    DEBUG,
    RELEASE_DEBUG_INFO,
    RELEASE,
};

enum class MODULE_TYPE {
    EXE,
    LIB,
    DLL
};

struct SolutionConfig {
    std::string                 code_style; // llama.codestyle
    std::vector<std::string>    modules;    // [folder1/llama.module, folder2/llama.module]
    MACHINE_ARCH                arch;
    TARGET_OS                   os;
    BUILD_TARGET                buildConfig;
    OPT_TARGET                  optimization;
    bool                        testsEnabled;
    bool                        reflectionEnabled;
    bool                        namespacesEnabled;
    bool                        methodsEnabled;
    bool                        lambdasEnabled;
    bool                        enforceCodeStyle;
};

struct ModuleConfig {
    std::string srcFolder;
    std::string moduleName;
    MODULE_TYPE moduleType;
    bool exportInfo;
};
