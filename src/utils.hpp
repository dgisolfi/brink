#pragma once
#include <iostream>
#include <filesystem>
#include <string_view>
#ifndef BRINK_UTILS_HPP
#define BRINK_UTILS_HPP

namespace brink {
        int file_exists(std::string path);
        int del_file(std::string path);
};

#endif // BRINK_UTILS_HPP