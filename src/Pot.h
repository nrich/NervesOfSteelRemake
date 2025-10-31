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

#ifndef POT_H
#define POT_H

#include <cstdint>
#include <array>
#include <fstream>

#include <raylib-cpp.hpp>

#include "Palette.h"

class Pot {
    raylib::TextureUnmanaged texture;

    uint16_t width;
    uint16_t height;

    void load(std::ifstream &fh, const Palette &palette);
public:
    Pot(const std::string &filename, const Palette &palette);
    Pot(std::ifstream &fh, const Palette &palette);

    const raylib::TextureUnmanaged &getTexture() const {
        return texture;
    }

    ~Pot();
};

#endif //POT_H
