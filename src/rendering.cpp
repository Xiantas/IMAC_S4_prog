#include "rendering.h"

auto Renderer::isProgramLoaded(std::filesystem::path const &prog) -> bool {
    for (auto const &t : this->programs) {
        if (std::get<0>(t) == prog) {
            return true;
        }
    }

    return false;
}
