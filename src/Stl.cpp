//
// Representation of a .stl file.
//

#pragma once

// project
#include "Stl.hpp"

// external
#include "stl_reader.h"
//#include "external/stl_reader.h"

// system
#include <iostream>

using bc::Stl;

bool Stl::from_file(const std::string_view& file, Stl& out_stl) {
    stl_reader::StlMesh <float, unsigned int> mesh;

    if (!mesh.read_file(file.data())) {
        std::cerr << "Could not open .stl file " << file << std::endl;
        return false;
    }

    out_stl.solids.resize(mesh.num_solids());

    size_t offset = 0;
    for (size_t isolid = 0; isolid < mesh.num_solids(); ++isolid) {
        out_stl.solids[isolid].facets.resize(mesh.solid_tris_end(isolid) - mesh.solid_tris_begin(isolid));

        for (size_t itri = mesh.solid_tris_begin(isolid); itri < mesh.solid_tris_end(isolid); ++itri) {
            auto& facet = out_stl.solids[isolid].facets[itri - offset];

            facet.normal = mesh.tri_normal(itri);

            facet.vertices[0] = mesh.tri_corner_coords(itri, 0);
            facet.vertices[1] = mesh.tri_corner_coords(itri, 1);
            facet.vertices[2] = mesh.tri_corner_coords(itri, 2);
        }

        offset += mesh.solid_tris_end(isolid) - mesh.solid_tris_begin(isolid);
    }

    return true;
}
