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
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <algorithm>
#include <cctype>

#include "Map.h"
#include "TextureCache.h"

struct MapSegment {
    uint32_t x1;
    uint32_t y1;
    uint32_t x2;
    uint32_t y2;

    uint32_t o5;
    uint32_t o6;
    uint32_t o7;
    uint32_t o8;

    uint32_t o9;
    uint32_t o10;
    uint32_t o11;
    uint32_t o12;

    uint32_t o13;
    uint32_t o14;
    uint32_t o15;
    uint32_t o16;

    uint32_t o17;
    uint32_t o18;
    uint32_t o19;
    uint32_t o20;

    uint32_t o21;
    uint32_t o22;
    uint32_t o23;
    uint32_t o24;

    uint32_t o25;
    uint32_t o26;
    uint32_t o27;
    uint32_t o28;

    uint32_t o29;
    uint32_t o30;
    uint32_t o31;
    uint32_t o32;

    uint32_t o33;
    uint32_t o34;
    uint32_t o35;
    uint32_t o36;

    uint32_t o37;
    uint32_t o38;
    uint32_t o39;
    uint32_t o40;

    uint32_t o41;
    uint32_t o42;
    uint32_t o43;
    uint32_t o44;

    uint32_t o45;
    uint32_t o46;
    uint32_t texture_id;   // texture?
    uint32_t o48;

    uint32_t o49;
    uint32_t o50;
    uint32_t o51;
    uint32_t o52;

    uint32_t o53;
    uint32_t o54;
    uint32_t o55;
    uint32_t o56;
};

size_t hash_map_segment(const MapSegment &map_segment) {
    uint64_t map_segment_data1 = ((uint64_t)(map_segment.x1) << 32) | (uint64_t)map_segment.y1;
    uint64_t map_segment_data2 = ((uint64_t)(map_segment.x2) << 32) | (uint64_t)map_segment.y2;

    return std::hash<uint64_t>{}(map_segment_data1 ^ map_segment_data2);
}

Map::Map(const std::string &filename) : filename(filename), x(0), y(0), width(0), height(0) {
    std::ifstream fh(filename, std::ios::binary|std::ios::in);

    if (!fh.is_open())
        throw std::runtime_error("Could not open file " + filename);

    uint32_t segment_count = 0; 
    uint32_t unknown1 = 0; 
    uint32_t unknown2 = 0; 
    uint32_t unknown3 = 0; 
    uint32_t texture_count = 0; 

    fh.read((char *)&segment_count, sizeof(segment_count));
    fh.read((char *)&unknown1, sizeof(unknown1));
    fh.read((char *)&unknown2, sizeof(unknown2));
    fh.read((char *)&unknown3, sizeof(unknown3));
    fh.read((char *)&texture_count, sizeof(texture_count));

    MapSegment map_segment;

    size_t map_hash_id = std::hash<std::string>{}(filename);

    for (uint32_t i = 0; i < segment_count; i += 1) {
        fh.read((char *)&map_segment, sizeof(map_segment));
        size_t segment_id = map_hash_id ^ hash_map_segment(map_segment);

        segments.push_back(Segment(segment_id, map_segment.x1, map_segment.y1, map_segment.x2, map_segment.y2, map_segment.texture_id));

        x = std::min(x, std::min(map_segment.x1, map_segment.x2));
        y = std::min(y, std::min(map_segment.y1, map_segment.y2));
        width = std::max(width, std::max(map_segment.x1, map_segment.x2));
        height = std::max(height, std::max(map_segment.y1, map_segment.y2));
    }

    for (uint32_t i = 0; i < texture_count; i += 1) {
        std::array<char, 24> texture_name_data;

        fh.read(texture_name_data.data(), texture_name_data.size());

        std::transform(std::begin(texture_name_data), std::end(texture_name_data), std::begin(texture_name_data), [](unsigned char c){ return c == '\\' ? '/' : std::toupper(c); });

        auto texture_name = "NOS/ENGINE/" + std::string(texture_name_data.data());

        //std::cerr << "Texture " << i << " " << texture_name << "\n";

        auto texture = TextureCache::LoadPot(texture_name);

        textures[i] = texture; 
    }
}

Map::~Map() {

}
