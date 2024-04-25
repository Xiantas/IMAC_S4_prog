#include "renderer.h"

#include "openGL/program.h"

namespace _fs = std::filesystem;

Renderer::Renderer(Camera camera)
    : camera(camera)
{}

auto Renderer::isProgramLoaded(
    _fs::path const &vert,
    _fs::path const &frag
) const -> std::optional<size_t> {
    for (size_t i = 0; i < this->programs.size(); ++i) {
        if (std::get<0>(this->programs[i]) == vert
            && std::get<1>(this->programs[i]) == frag)
        {
            return i;
        }
    }

    return {};
}

auto Renderer::getProgramIndex(_fs::path const &vert, _fs::path const &frag) -> size_t {
    auto val = this->isProgramLoaded(vert, frag);
    if (val) {
        return val.value();
    } else {
        GLshader vsShader(GL_VERTEX_SHADER), fsShader(GL_FRAGMENT_SHADER);
        vsShader.setCodeFromFile(vert);
        fsShader.setCodeFromFile(frag);

        this->programs.emplace_back(vert, frag, GLprogram());
        std::get<2>(this->programs.back())
            .addShader(vsShader)
            .addShader(fsShader)
            .link();

        return this->programs.size()-1;
    }
}

auto Renderer::getProgramRef(size_t index) const -> GLprogram const& {
    return std::get<2>(this->programs.at(index));
}
