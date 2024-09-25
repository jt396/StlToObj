//
// Representation of a .obj file.
//

#pragma once

// project
// ...

// external
// ...

// system
#include <vector>
#include <string_view>

namespace bc {
    struct Stl;

    struct Obj final {
        struct Vertex final {
            Vertex() = default;
            Vertex(float x_, float y_, float z_) : x{ x_ }, y{ y_ }, z{ z_ } {}

            float x;
            float y;
            float z;
        };

        struct Face final {
            int vertex0;
            int vertex1;
            int vertex2;
        };

        std::vector<Vertex> vertices;
        std::vector<Face>   indices;

        // Create an Obj object from a given Stl object.
        [[nodiscard]] static bool from_stl(const bc::Stl& stl, bc::Obj& out_obj, double tolerance) noexcept;

        // Send an Obj to a specified file.
        [[nodiscard]] bool to_file(const std::string_view& file) const noexcept;
    };
}
