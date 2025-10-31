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

#include "Cel.h"

Cel::Cel(const std::string &filename) {
    std::ifstream fh(filename, std::ios::binary|std::ios::in);

    load(fh);
}

Cel::Cel(std::ifstream &fh) {
    load(fh);
}

void Cel::load(std::ifstream &fh) {
    std::vector<uint8_t> pixels;

    uint16_t magic;
    std::array<char, 26> skip;

    fh.read((char *)&magic, sizeof(magic));

    if (magic != 0x9119)
        throw std::domain_error("Incorrect magic value " + std::format("{:#04x}", magic));

    fh.read((char *)&width, sizeof(width));
    fh.read((char *)&height, sizeof(height));
    fh.read(skip.data(), skip.size());

    Palette palette(fh);

    while (!fh.eof()) {
        uint8_t index;

        fh.read((char *)&index, sizeof(index));

        auto pixel = palette[index];

        pixels.push_back(pixel.GetR());
        pixels.push_back(pixel.GetG());
        pixels.push_back(pixel.GetB());
        pixels.push_back(pixel.GetA());
    }

    auto bytes = new uint8_t[pixels.size()];
    std::memcpy(bytes, pixels.data(), pixels.size());

    auto image = raylib::Image(bytes, width, height, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

    texture = raylib::TextureUnmanaged(image);
    texture.SetWrap(TEXTURE_WRAP_CLAMP);
}

Cel::~Cel() {

}
