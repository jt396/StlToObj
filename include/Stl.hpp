//
// Representation of a .stl file.
//

#pragma once

// project
// ...

// external
// ...

// system
#include <vector>
#include <string_view>
#include <array>

namespace bc {
    struct Obj;

    struct Stl final {
        struct Vector final {
            Vector() = default;
            Vector(const float* data) : x{data[0]}, y{data[1]}, z{data[2]} {}

            float x;
            float y;
            float z;
        };

        struct Solid final {
            struct Facet final {
                Facet() = default;

                Vector normal;
                std::array<Vector, 3> vertices;
            };
            std::vector<Facet> facets;
        };

        std::vector<Solid> solids;

        // Create an Stl object from a given file name.
        [[nodiscard]] static bool from_file(const std::string_view& file, Stl& out_stl);
    };
}
