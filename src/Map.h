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

#ifndef MAP_H
#define MAP_H

#include <cstdint>
#include <array>
#include <exception>
#include <string>
#include <unordered_map>

#include <raylib-cpp.hpp>

#include "Segment.h"

class World;

class Map {
    const std::string filename;

    uint32_t x;
    uint32_t y;

    uint32_t width;
    uint32_t height;

    std::vector<Segment> segments;
    std::unordered_map<uint32_t, const raylib::TextureUnmanaged*> textures;
public:
    Map(const std::string &filename);

    const std::vector<Segment> &getSegments() const {
        return segments;
    }

    const std::string &getFilename() const {
        return filename;
    }

    const raylib::TextureUnmanaged *getTexture(uint32_t texture_id) const {
        return textures.at(texture_id);
    }

    ~Map();
};

#endif //MAP_H
