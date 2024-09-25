//
// Representation of a .obj file.
//

// project
#include "Obj.hpp"
#include "Stl.hpp"

// external
// ...

// system
#include <iostream>
#include <fstream>
#include <cmath>

using bc::Obj;
using bc::Stl;

namespace {
    // 1. Subtract to get a vector between the vertices
    // 2. Take the magnitude of that vector to work out how close together they are
    [[nodiscard]] double distance_between_vertices(const Obj::Vertex& v0, const Obj::Vertex& v1) noexcept {
        const auto new_vertex = Obj::Vertex(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
        const auto magnitude  = std::sqrt(new_vertex.x * new_vertex.x + new_vertex.y * new_vertex.y + new_vertex.z * new_vertex.z);

        return magnitude;
    }

    [[nodiscard]] std::size_t insert_vertex(const Obj::Vertex vertex, double tolerance, std::vector<Obj::Vertex>& out_vertices, int& out_removed_vertex_count) {
        if (tolerance > 0.0) {
            // Only insert it if there is no other vertex in this position within the specified tolerance.
            for (auto i=0u; i< out_vertices.size(); ++i) {
                if (const auto distance = distance_between_vertices(out_vertices[i], vertex); distance <= tolerance) {
                    // Already have a suitable vertex
                    ++out_removed_vertex_count;
                    return i + 1;
                }
            }

            out_vertices.push_back(vertex);
            return out_vertices.size();
        } else {
            // Just insert it (don't care about removing overlapping vertices).
            out_vertices.push_back(vertex);
            return out_vertices.size();
        }
    }
}

bool Obj::from_stl(const Stl& stl, bc::Obj& out_obj, double tolerance) noexcept {
    int removed_vertices = 0;
    int index = 0;

    // TODO: We'll need to think of a faster way to do this - it's incredibly slow for complex meshes...
    for (const auto& solid : stl.solids) {
        for (const auto& facet : solid.facets) {
            const auto index0 = insert_vertex(Obj::Vertex(facet.vertices[0].x, facet.vertices[0].y, facet.vertices[0].z), tolerance, out_obj.vertices, removed_vertices);
            const auto index1 = insert_vertex(Obj::Vertex(facet.vertices[1].x, facet.vertices[1].y, facet.vertices[1].z), tolerance, out_obj.vertices, removed_vertices);
            const auto index2 = insert_vertex(Obj::Vertex(facet.vertices[2].x, facet.vertices[2].y, facet.vertices[2].z), tolerance, out_obj.vertices, removed_vertices);

            out_obj.indices.push_back(Obj::Face(index0, index1, index2));
        }
    }

    if (tolerance != 0.0) {
        std::cout << "Merged " << removed_vertices << " vertices with a tolerance/distance of " << tolerance << std::endl;
    }

    return true;
}

bool Obj::to_file(const std::string_view& file) const noexcept {
    std::ofstream fstream;
    fstream.open(file.data());

    if (!fstream.is_open()) {
        std::cerr << "Could not open/create .obj file " << file << std::endl;
        return false;
    }

    for (const auto& vertex : vertices) {
        fstream << "v " << vertex.x << " "
                        << vertex.y << " "
                        << vertex.z << "\n";
    }

    for (const auto& index : indices) {
        fstream << "f " << index.vertex0 << " "
                        << index.vertex1 << " "
                        << index.vertex2 << "\n";
    }

    return true;
}
