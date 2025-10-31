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

#ifndef CEL_H
#define CEL_H

#include <cstdint>
#include <array>
#include <fstream>

#include <raylib-cpp.hpp>

#include "Palette.h"

class Cel {
    raylib::TextureUnmanaged texture;

    uint16_t width;
    uint16_t height;

    void load(std::ifstream &fh);
public:
    Cel(const std::string &filename);
    Cel(std::ifstream &fh);

    const raylib::TextureUnmanaged &getTexture() const {
        return texture;
    }

    ~Cel();
};

#endif //CEL_H
