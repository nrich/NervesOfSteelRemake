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

#ifndef PLAYER_H
#define PLAYER_H

#include <cstdint>
#include <unordered_map>
#include <functional>
#include <optional>
#include <utility>
#include <vector>

#include <iostream>

#include <raylib-cpp.hpp>

#include "Game.h"

class Player {
    raylib::Vector2 angles;

    raylib::Camera camera;

    State state;

    uint64_t input = 0;

    std::pair<raylib::Vector3, raylib::Vector3> processInput(const uint64_t frame_count);
    void tryMove(const raylib::Vector3 &movement, const raylib::Vector3 &rotation);

    int radius = 3;
public:
    Player();

    raylib::Vector2 getPosition() const {
        auto camera_position = camera.GetPosition();
        return raylib::Vector2(camera_position.x, camera_position.z);
    }

    void setPosition(const raylib::Vector2 &new_position) {
        camera.SetPosition(raylib::Vector3(new_position.GetX(), 6.0f, new_position.GetY()));
        camera.SetTarget(raylib::Vector3(new_position.GetX(), 6.0f, new_position.GetY() - 10));
        angles = raylib::Vector2();
    }

    raylib::Vector2 getAngles() const {
        return angles;
    }

    void setAngles(const raylib::Vector2 &new_angles) {
        camera.Update(raylib::Vector3(), raylib::Vector3(-angles.GetX(), 0.0f, 0.0f));
        camera.Update(raylib::Vector3(), raylib::Vector3(new_angles.GetX(), 0.0f, 0.0f));

        angles = new_angles;
    }

    raylib::Camera *getCamera() {
        return &camera;
    }

    State getState() const {
        return state;
    }

    void setState(State new_state) {
        state = new_state;
        input = 0;
    }

    void update(const uint64_t frame_count);

    uint64_t getInput() const {
        return input;
    }

    void setInput(uint64_t new_input) {
        input = new_input;
    }

    ~Player();
};

#endif //PLAYER_H
