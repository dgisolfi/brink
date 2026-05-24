#include "utils.hpp"

namespace brink {
    
    int file_exists(std::string path) {
        if (!std::filesystem::exists(path)) {
            std::cerr << "File does not exist: " << path << std::endl;
            return 1;
        }
        return 0;
    }
int del_file(std::string path) {
    
        try {
            if (std::filesystem::remove(path)) {
                std::cout << "File deleted successfully.\n";
            } else {
                std::cout << "File not found, nothing to delete.\n";
            }
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error: " << e.what() << '\n';
        }

        return 0;
    }

};