#include "utils.hpp"

namespace brink {
    
    int file_exists(std::string path) {
        // if (!std::filesystem::exists(path)) {
        //     return 1;
        // }
        return 0;
    }

    int del_file(std::string path) {
        // TODO: setup error codes
        // std::error_code ec;
        // if (!std::filesystem::remove(path)) {
        //     return 1;
        // }

        if (std::remove(path.c_str()) != 0) {
            return 1;
        };
        return 0;
    }

};