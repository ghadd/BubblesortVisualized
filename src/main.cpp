#include "visualization.hpp"
#include <memory>

auto main() -> int {
  std::unique_ptr<Visualization> vs =
      std::make_unique<Visualization>("config.yml");

  if (vs->Construct(vs->getRows(), vs->getCols(), 4, 4, vs->m_Maximized, true))
    vs->Start();

  return 0;
}