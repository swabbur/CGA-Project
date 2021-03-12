#include <util/Files.hpp>
#include <fstream>
#include <stdexcept>

std::string Files::read_text(std::string const & path) {

    // Open file
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Could not load file: " + path);
    }

    // Read text
    std::string text;
    file.seekg(0, std::ios::end);
    text.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&text[0], text.size());

    // Close file
    file.close();

    return text;
}