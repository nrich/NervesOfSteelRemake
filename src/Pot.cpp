/******************************************************************************

Copyright (C) 2025 Neil Richardson (nrich@neiltopia.com)

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, version 3.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
details.

You should have received a copy of the GNU General Public License along with
this program. If not, see <https://www.gnu.org/licenses/>.

******************************************************************************/

#include <filesystem>
#include <vector>
#include <iostream>
#include <format>
#include <cstring>

#include "Pot.h"

Pot::Pot(const std::string &filename, const Palette &palette) {
    std::ifstream fh(filename, std::ios::binary|std::ios::in);

    if (!fh.is_open())
        throw std::runtime_error("Could not open file " + filename);

    load(fh, palette);
}

Pot::Pot(std::ifstream &fh, const Palette &palette) {
    load(fh, palette);
}

void Pot::load(std::ifstream &fh, const Palette &palette) {
    std::vector<uint8_t> pixels;

    uint32_t magic;
    uint32_t compressed_size;
    uint32_t decompressed_size;
    uint32_t check;

    fh.read((char *)&magic, sizeof(magic));
    fh.read((char *)&compressed_size, sizeof(compressed_size));
    fh.read((char *)&decompressed_size, sizeof(decompressed_size));
    fh.read((char *)&check, sizeof(check));

    std::cerr << std::format("{:#08x}", check) << "\n";

    // POT\x00
    if (magic != 0x00544F50)
        throw std::domain_error("Incorrect magic value " + std::format("{:#08x}", magic));

    // \x2ECEL or \x2Ecel
    if (check != 0x4C45432E && check != 0x6C65632E)
        throw std::domain_error("Incorrect check value " + std::format("{:#08x}", check));

    while (pixels.size() < decompressed_size*4) {
        uint8_t control = 0;

        fh.read((char *)&control, sizeof(control));
        if (fh.eof())
            break;

        if (control > 0x80) {
            uint8_t literal_count = control - 0x80;

            for (uint8_t i = 0; i < literal_count; i++) {
                uint8_t index = 0;
                fh.read((char *)&index, sizeof(index));

                auto pixel = palette[index];

                pixels.push_back(pixel.GetR());
                pixels.push_back(pixel.GetG());
                pixels.push_back(pixel.GetB());
                pixels.push_back(pixel.GetA());
            }
        } else {
            uint8_t index = 0;
            fh.read((char *)&index, sizeof(index));

            uint8_t copy_count = control;

            for (uint8_t i = 0; i < copy_count; i++) {
                auto pixel = palette[index];

                pixels.push_back(pixel.GetR());
                pixels.push_back(pixel.GetG());
                pixels.push_back(pixel.GetB());
                pixels.push_back(pixel.GetA());
            } 
        }
    }

    if (pixels.size() != decompressed_size*4)
        throw std::runtime_error("Decompression error: expected " + std::to_string(decompressed_size) + ", got " + std::to_string(pixels.size()/4));

    switch (decompressed_size) {
        case 320*200:
            width = 320;
            height = 200;
            break;
        case 320*40:
            width = 320;
            height = 40;
            break;
        case 11*11:
            width = 11;
            height = 11;
            break;
        case 128*128:
            width = 128;
            height = 128;
            break;
        default:
            throw std::runtime_error("Invalid POT size: " + std::to_string(decompressed_size));
    }

    auto bytes = new uint8_t[pixels.size()];
    std::memcpy(bytes, pixels.data(), pixels.size());

    auto image = raylib::Image(bytes, width, height, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

    texture = raylib::TextureUnmanaged(image);
    //texture.SetWrap(TEXTURE_WRAP_CLAMP);
    texture.SetWrap(TEXTURE_WRAP_REPEAT);
}

Pot::~Pot() {

}
