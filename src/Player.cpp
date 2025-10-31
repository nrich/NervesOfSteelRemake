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

#include "Player.h"

#include <iostream>

#ifndef M_PI
#define M_PI            3.14159265358979323846
#endif

#ifndef DEG2RAD
#define DEG2RAD (M_PI/180.0)
#endif

#ifndef RAD2DEG
#define RAD2DEG (180.0f/M_PI)
#endif

Player::Player() : angles(0, 0), state(State::World) {
    camera = raylib::Camera(
        raylib::Vector3(0, 6.0f, 0),
        raylib::Vector3(0, 6.0f, 0),
        raylib::Vector3(0.0f, 1.0f, 0.0f),
        45.0f,
        CAMERA_PERSPECTIVE
    );

    setPosition(raylib::Vector2(0, 0));
}

std::pair<raylib::Vector3, raylib::Vector3> Player::processInput(const uint64_t frame_count) {
    uint64_t player_input = this->getInput();

    const float offset_this_frame = 60.0f * GetFrameTime();
    const float x_rotate_this_frame = 120.0f * GetFrameTime();
    const float y_rotate_this_frame = 90.0f * GetFrameTime();

    raylib::Vector3 movement(0, 0, 0);
    raylib::Vector3 rotation(0, 0, 0);

    if (player_input & Input::StepForward) {
        movement.SetX(offset_this_frame);
    } else if (player_input & Input::StepBack) {
        movement.SetX(-offset_this_frame);
    }

    if (player_input & Input::StepLeft) {
        movement.SetY(-offset_this_frame);
    } else if (player_input & Input::StepRight) {
        movement.SetY(offset_this_frame);
    }

    if (player_input & Input::TurnLeft) {
        rotation.SetX(-x_rotate_this_frame);
    } else if (player_input & Input::TurnRight) {
        rotation.SetX(x_rotate_this_frame);
    }

    if (player_input & Input::LookDown) {
        rotation.SetY(y_rotate_this_frame);
    } else if (player_input & Input::LookUp) {
        rotation.SetY(-y_rotate_this_frame);
    }

    if (player_input & Input::PrimaryAction) {
        //this->useWeapon(frame_count);
    }

    if (player_input & Input::Use) {
        //this->use(frame_count);
    }

    this->setInput(player_input);

    return std::make_pair(movement, rotation);
}


void Player::tryMove(const raylib::Vector3 &movement, const raylib::Vector3 &rotation) {
    auto camera_position = camera.GetPosition();

    if (rotation.Length()) {
        angles += raylib::Vector2(rotation.GetX(), rotation.GetY());

        if (angles.GetX() < 0) {
            angles.x += 360.0f;
        }

        if (angles.GetX() >= 360.0f) {
            angles.x -= 360.0f;
        }


        if (angles.GetY() > 25) {
            angles.y = 25.0f;
        }

        if (angles.GetY() < -25) {
            angles.y = -25.0f;
        }

        camera.Update(raylib::Vector3(), raylib::Vector3(rotation.GetX(), rotation.GetY(), 0.0f), 0.0);
    }

    if (movement.Length()) {
        auto position = raylib::Vector2(camera_position.x, camera_position.z);

        raylib::Vector2 new_move(movement.y, -movement.x);
        new_move = new_move.Rotate(DEG2RAD * angles.GetX());

        auto new_position = position + new_move;
        auto new_movement = movement;

        camera.Update(new_movement, raylib::Vector3(), 0.0);
        auto camera_position2 = camera.GetPosition();
    }


#if 0
    auto *world = getWorld();
    auto *level = world->getCurrentLevel();
    auto *map = level->getMap();

    auto camera_position = camera.GetPosition();

    if (rotation.Length()) {
        angles += raylib::Vector2(rotation.GetX(), 0.0f);

        if (angles.GetX() < 0) {
            angles.x += 360.0f;
        }

        if (angles.GetX() >= 360.0f) {
            angles.x -= 360.0f;
        }
 
        camera.Update(raylib::Vector3(), raylib::Vector3(rotation.GetX(), 0.0f, 0.0f), 0.0);
    }

    if (movement.Length()) {
        auto position = raylib::Vector2(camera_position.x, camera_position.z);

        raylib::Vector2 new_move(movement.y, -movement.x);
        new_move = new_move.Rotate(DEG2RAD * angles.GetX());

        auto new_position = position + new_move;
        auto new_movement = movement;

        bool reset = false;

        for (const auto &segment : map->getSegments()) {
            Entity *entity = world->getEntity(segment.id);
            if (entity) {
                auto bounds_if = entity->getBounds();
                if (bounds_if) {
                    auto bounds = *bounds_if;

                    float distance = bounds.first.Distance(new_position);
                    if (bounds.second + radius > distance) {
                        auto collision = entity->collide();

                        if (collision == Collision::Block) {
                            reset = true;
                        } else if (collision == Collision::Touch) {
                            entity->touch(this);
                            reset = true;
                        }
                    }

                    continue;
                }

                // horizontal segments
                if (segment.y1 == segment.y2) {
                    uint16_t x1 = segment.x1;
                    uint16_t x2 = segment.x2;
                    uint16_t y1 = segment.y1;

                    if (x1 > x2) {
                        std::swap(x1, x2);
                    }

                    if (new_position.x >= x1 && new_position.x <= x2) {
                        int north = new_position.y - radius;
                        int south = new_position.y + radius;

                        if (y1 < new_position.y && y1 > north) {
                            auto collision = entity->collide();
                            if (collision == Collision::Block) {
                                reset = true;
                            } else if (collision == Collision::Touch) {
                                reset = true;
                                entity->touch(this);
                            }

                            break;
                        }

                        if (y1 > new_position.y && y1 < south) {
                            auto collision = entity->collide();
                            if (collision == Collision::Block) {
                                reset = true;
                            } else if (collision == Collision::Touch) {
                                reset = true;
                                entity->touch(this);
                            }
                            break;
                        }
                    }
                }

                // vertical segments
                if (segment.x1 == segment.x2) {
                    uint16_t y1 = segment.y1;
                    uint16_t y2 = segment.y2;
                    uint16_t x1 = segment.x1;

                    if (y1 > y2) {
                        std::swap(y1, y2);
                    }

                    if (new_position.y >= y1 && new_position.y <= y2) {
                        int west = new_position.x - radius;
                        int east = new_position.x + radius;

                        if (x1 < new_position.x && x1 > west) {
                            auto collision = entity->collide();
                            if (collision == Collision::Block) {
                                reset = true;
                            } else if (collision == Collision::Touch) {
                                reset = true;
                                entity->touch(this);
                            }
                            break;
                        }

                        if (x1 > new_position.x && x1 < east) {
                            auto collision = entity->collide();
                            if (collision == Collision::Block) {
                                reset = true;
                            } else if (collision == Collision::Touch) {
                                reset = true;
                                entity->touch(this);
                            }
                            break;
                        }
                    }
                }
            }
        }

        if (reset) {
            new_movement = raylib::Vector3();
        }

        camera.Update(new_movement, raylib::Vector3(), 0.0);
    }
#endif
}
 
void Player::update(const uint64_t frame_count) {
    auto [movement, rotation] = processInput(frame_count);

    tryMove(movement, rotation);
}

Player::~Player() {

}
