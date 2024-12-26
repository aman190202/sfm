#include <iostream>
#include <vector>
#include <cstdint>
#include <dirent.h>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


int main(int argc, char* argv[]) 
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <folder_path>" << std::endl;
        return 1;
    }

    std::string folder_path = argv[1];

    // Open the directory
    DIR* dir = opendir(folder_path.c_str());
    if (!dir) {
        std::cerr << "Error: Unable to open folder " << folder_path << std::endl;
        return 1;
    }

    struct dirent* entry;
    std::vector<uint8_t*> rgb_images;

    while ((entry = readdir(dir)) != nullptr) {
        std::string file_name = entry->d_name;

        // Skip special entries "." and ".."
        if (file_name == "." || file_name == "..") continue;

        std::string image_path = folder_path + "/" + file_name;

        // Load the image
        int width, height, bpp;
        uint8_t* rgb_image = stbi_load(image_path.c_str(), &width, &height, &bpp, 3);
        if (rgb_image) {
            std::cout << "Loaded image: " << image_path 
                      << " (Width: " << width 
                      << ", Height: " << height 
                      << ", Channels: 3)" << std::endl;
            rgb_images.push_back(rgb_image);
        } else {
            std::cerr << "Failed to load image: " << image_path << std::endl;
        }
    }

    closedir(dir);

    // Process the loaded images
    std::cout << "Total images loaded: " << rgb_images.size() << std::endl;

    // Free the memory allocated for each image
    for (auto image : rgb_images) {
        stbi_image_free(image);
    }

    return 0;
}