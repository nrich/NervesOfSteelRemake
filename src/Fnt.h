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

#ifndef FNT_H
#define FNT_H

#include <cstdint>
#include <string>
#include <optional>
#include <unordered_map>

#include <raylib-cpp.hpp>

class Fnt {
    std::unordered_map<char, raylib::TextureUnmanaged> font_map;

    std::string filename;

    uint8_t width;
    uint8_t height;

    std::string name;

    raylib::TextureUnmanaged getChar(char c);
public:
    Fnt(const std::string &filename);
    void write(const std::string &text, int x, int y, int scale, std::optional<raylib::Color> tint=std::nullopt, float rotation=0.0f);
    ~Fnt();
};

#endif //FNT_H

