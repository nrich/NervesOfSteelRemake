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
#include <fstream>
#include <exception>
#include <iostream>

#include "Palette.h"

Palette::Palette(const std::string &filename) {
    std::filesystem::path filepath = filename;

    if (std::filesystem::file_size(filepath) != PALETTE_BUFFER_LEN)
        throw std::invalid_argument(filename + " does not look like a VGA palette file"); 

    std::ifstream fh(filename, std::ios::binary|std::ios::in);

    if (!fh.is_open())
        throw std::runtime_error("Could not open file " + filename);

    read(fh);
}

Palette::Palette(std::ifstream &fh) {
    read(fh);
}

Palette::Palette(const std::array<uint8_t, PALETTE_BUFFER_LEN> &data) {
    load(data);
}

void Palette::load(const std::array<uint8_t, PALETTE_BUFFER_LEN> &data) {
    for (size_t i = 0, c = 0; i < 256; i += 1, c += 3) {
        uint8_t r, g, b, a;

        r = data[c];
        g = data[c+1];
        b = data[c+2];

        if (i == 0) {
            a = 0x00;
        } else {
            a = 0xFF;
        }

        colours[i] = raylib::Color((r * 255) / 63, (g * 255) / 63, (b * 255) / 63, a);
    }
}

void Palette::read(std::ifstream &fh) {
    std::array<uint8_t, PALETTE_BUFFER_LEN> data;

    fh.read((char *)&data, data.size());

    if (data.size() < PALETTE_BUFFER_LEN)
        throw std::runtime_error("Short palette buffer read: " + std::to_string(data.size()));

    load(data);
}

Palette::~Palette() {
}
