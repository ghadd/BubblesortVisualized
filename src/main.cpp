// #define MAXIMIZED

#include "visualization.hpp"

auto main() -> int
{
    auto *vs = new Visualization("config.yml");

#ifdef MAXIMIZED
    bool bIsMaximized = true;
#else
    bool bIsMaximized = false;
#endif

    if (vs->Construct(vs->getRows(), vs->getCols(), 4, 4, bIsMaximized))
        vs->Start();

    return 0;
}