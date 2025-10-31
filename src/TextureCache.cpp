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

#include <unordered_map>
#include <iostream>

#include "Cel.h"
#include "Pot.h"
#include "TextureCache.h"

static std::unordered_map<std::string, raylib::TextureUnmanaged> cache;

const raylib::TextureUnmanaged *TextureCache::LoadCel(const std::string &filename) {
    if (cache.contains(filename)) {
        return &cache[filename];
    }

    raylib::TextureUnmanaged texture = Cel(filename).getTexture();

    cache.emplace(filename, texture);

    return &cache[filename];
}

const raylib::TextureUnmanaged *TextureCache::LoadPot(const std::string &filename) {
    static Palette palette("NOS/ENGINE/NOS.PAL");

    if (cache.contains(filename)) {
        return &cache[filename];
    }

    raylib::TextureUnmanaged texture = Pot(filename, palette).getTexture();
    cache.emplace(filename, texture);

    return &cache[filename];
}

const std::vector<const raylib::TextureUnmanaged*> TextureCache::LoadCel(const std::vector<std::string> &filenames) {
    std::vector<const raylib::TextureUnmanaged*> textures;

    for (const auto &filename : filenames) {
        textures.push_back(TextureCache::LoadCel(filename));
    }

    return textures;
}

const std::vector<const raylib::TextureUnmanaged*> TextureCache::LoadPot(const std::vector<std::string> &filenames) {
    std::vector<const raylib::TextureUnmanaged*> textures;

    for (const auto &filename : filenames) {
        textures.push_back(TextureCache::LoadPot(filename));
    }

    return textures;
}
