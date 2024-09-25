//
// main entry point
//

// project
#include "Stl.hpp"
#include "Obj.hpp"

// external
// ...

// system
#include <iostream>
#include <filesystem>

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "Usage: StlToObj.exe input.stl output.obj tolerance" << std::endl;
        return -1;
    }

    // Argument validation
    const std::string src_file_name { argv[1] };
    const std::string dst_file_name { argv[2] };
    const double tolerance { std::stod(argv[3]) };

    if (!std::filesystem::exists(src_file_name)) {
        std::cerr << "Source file: " << src_file_name << " could not be found..." << std::endl;
        return -1;
    }

    // Read in .stl
    bc::Stl stl;
    if (!bc::Stl::from_file(src_file_name, stl)) {
        std::cerr << "Failed to initialise stl instance from file " << src_file_name << " - quitting..." << std::endl;
        return -1;
    }

    // Create obj instance from stl instance
    bc::Obj obj;
    if (!bc::Obj::from_stl(stl, obj, tolerance)) {
        std::cerr << "Failed to initialise obj instance from stl instance - quitting..." << std::endl;
        return -1;
    }

    // Send to .obj
    if (!obj.to_file(dst_file_name)) {
        std::cerr << "Encountered an error writing out to " << dst_file_name << std::endl;
        return -1;
    }

    std::cout << "Done, " << dst_file_name << " successfully created." << std::endl;

    return 0;
}
