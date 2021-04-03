#pragma once

#include "Config.hpp"

namespace llang {

    struct Compiler {

        static int compile(SolutionConfig &solutionConfig);

        static int compile(ModuleConfig &moduleConfig);
    };
}
