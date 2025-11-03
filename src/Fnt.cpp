#include <exception>
#include <fstream>
#include <array>
#include <cstring>
#include <iostream>

#include "Fnt.h"

Fnt::Fnt(const std::string &filename) : filename(filename), width(0), height(0) {
    std::ifstream fh(filename, std::ios::binary|std::ios::in);

    uint16_t char_count = 0;

    fh.read((char *)&height, 1);
    fh.read((char *)&width, 1);
    fh.read((char *)&char_count, 2);

    std::array<char, 16> name_data;

    fh.read(name_data.data(), name_data.size());

    name = std::string(name_data.data(), name_data.size());

    // There is a bug in file data
    if (height == 9)
        height = 5;

    for (int i = 0; i < char_count; i++) {
        std::vector<uint8_t> pixels;
        char chr = 1;

        fh.read((char *)&chr, 1);

        for (int j = 0; j < height*width; j++) {
            uint8_t b;

            fh.read((char *)&b, 1);

            if (b) {
                pixels.push_back(0xFF);
                pixels.push_back(0xFF);
                pixels.push_back(0xFF);
                pixels.push_back(0xFF);
            } else {
                pixels.push_back(0x00);
                pixels.push_back(0x00);
                pixels.push_back(0x00);
                pixels.push_back(0x00);
            }
        }

        auto bytes = new uint8_t[pixels.size()];
        std::memcpy(bytes, pixels.data(), pixels.size());

        auto image = raylib::Image(bytes, width, height, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

        auto texture = raylib::TextureUnmanaged(image);

        font_map[chr] = texture;
    }
}

raylib::TextureUnmanaged Fnt::getChar(char c) {
    return font_map[c];
}

void Fnt::write(const std::string &text, int x, int y, int scale, std::optional<raylib::Color> tint, float rotation) {
    if (!tint)
        tint = raylib::Color(0xCF, 0xCD, 0x50, 0xFF);

    for (size_t i = 0; i < text.size(); i++) {
        auto texture = getChar(text[i]);
        texture.Draw(Vector2(x + (i * scale * width), y), rotation, scale, *tint);
    }
}

Fnt::~Fnt() {

}

