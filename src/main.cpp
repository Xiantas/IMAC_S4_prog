#include <cstdlib>
#include <vector>
#include <span>

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

#include "p6/p6.h"

#include "boid.h"
#include <iostream>

constexpr size_t N = 100;

int main()
{
    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    std::vector<Boid> crowd;
    crowd.resize(N);
    for (Boid &boid : crowd) {
        boid = Boid {
            .position = p6::random::point(),
            .direction = p6::random::point(),
            .speed = 0.0045,
            .detectionRadius = .25,
            .dodgeRadius = .13,
        };
    }

    // Actual application code
    auto ctx = p6::Context{{.title = "Simple-p6-Setup"}};
    ctx.maximize_window();

    // Declare your infinite update loop.
    ctx.update = [&]() {
        if (ctx.mouse_button_is_pressed(p6::Button::Left)) {
            std::cout << '(' << ctx.mouse().x << ", " << ctx.mouse().y << ")\n";
        }
        ctx.background(p6::NamedColor::Wheat);
        for (Boid &boid : crowd) {
            boid.update(std::span(crowd));
            if (boid.position.x < -1) boid.position.x =  0.99;
            if (boid.position.x >  1) boid.position.x = -0.99;
            if (boid.position.y < -1) boid.position.y =  0.99;
            if (boid.position.y >  1) boid.position.y = -0.99;
        }
        for (Boid const &boid : crowd) {
            ctx.equilateral_triangle(
                p6::Center{boid.position},
                p6::Radius{0.020f}
            );
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}
