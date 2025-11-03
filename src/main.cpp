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

#include <cstdint>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <string>
#include <iterator>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <memory>
#include <filesystem>
#include <chrono>
#include <thread>
#include <cmath>

#include <cmdline.h>
#include <raylib-cpp.hpp>

#include "Fnt.h"
#include "Game.h"
#include "Player.h"
#include "TextureCache.h"
#include "Map.h"
#include "LaunchOptions.h"

static const char *fragment_shader = R"(
#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

void main()
{
    vec4 texelColor = texture(texture0, fragTexCoord);
    if (texelColor.a == 0.0) discard;
    finalColor = texelColor * fragColor * colDiffuse;
})";

static void draw_entity(const raylib::Camera3D *camera, const uint16_t x1, const uint16_t y1, const uint16_t x2, const uint16_t y2, const raylib::TextureUnmanaged &texture) {
    const float y_offset = 5.0f;
    const float scale = 10.0f;

    float mid_x = x1;
    float mid_y = y1;

    static raylib::ShaderUnmanaged shader = raylib::ShaderUnmanaged::LoadFromMemory(nullptr, fragment_shader);

    shader.BeginMode();
    {
        camera->DrawBillboard(texture, Vector3(mid_x, y_offset, mid_y), scale);
    }
    shader.EndMode();
}

static void draw_wall(const uint32_t x1, const uint32_t y1, const uint32_t x2, const uint32_t y2, const raylib::TextureUnmanaged *texture) {
    const float height = 10.0f;

    static raylib::ShaderUnmanaged shader = raylib::ShaderUnmanaged::LoadFromMemory(nullptr, fragment_shader);

    rlColor4ub(0xFF, 0xFF, 0xFF, 0xFF);

    shader.BeginMode();
    {
        rlBegin(RL_TRIANGLES);
        rlSetTexture(texture->id);

        if (y1 == y2) {
            float u = (std::max(x2, x1) - std::min(x1, x2))/10.0f;

            rlTexCoord2f(0.0, 1.0);
            rlVertex3f(x1, 0, y1);

            rlTexCoord2f(u, 1.0);
            rlVertex3f(x2, 0, y1);

            rlTexCoord2f(u, 0.0);
            rlVertex3f(x2, height, y1);

            rlTexCoord2f(u, 0.0);
            rlVertex3f(x2, height, y1);

            rlTexCoord2f(0.0, 0.0);
            rlVertex3f(x1, height, y1);

            rlTexCoord2f(0.0, 1.0);
            rlVertex3f(x1, 0, y1);
        } else if (x1 == x2) {
            float u = (std::max(y2, y1) - std::min(y1, y2))/10.0f;

            rlTexCoord2f(0.0, 1.0);
            rlVertex3f(x1, 0, y1);

            rlTexCoord2f(u, 1.0);
            rlVertex3f(x1, 0, y2);

            rlTexCoord2f(u, 0.0);
            rlVertex3f(x1, height, y2);

            rlTexCoord2f(u, 0.0);
            rlVertex3f(x1, height, y2);

            rlTexCoord2f(0.0, 0.0);
            rlVertex3f(x1, height, y1);

            rlTexCoord2f(0.0, 1.0);
            rlVertex3f(x1, 0, y1);
        } else {
            //float u = (std::max(y2, y1) - std::min(y1, y2))/10.0f;
            float u = 1.0;
            //std::cerr << x1 << "," << y1 << " " << x2 << "," << y2 << "\n";

            uint32_t x = (std::max(x2, x1) - std::min(x1, x2));
            uint32_t y = (std::max(y2, y1) - std::min(y1, y2));

            //std::cerr << x << "," << y << "\n";

            u = std::sqrt((x*x) + (y*y));

            u /= 10.0f;
            u = std::round(u);

            //std::cerr << u << "\n";

            rlTexCoord2f(0.0, 1.0);
            rlVertex3f(x1, 0, y1);

            rlTexCoord2f(u, 1.0);
            rlVertex3f(x2, 0, y2);

            rlTexCoord2f(0.0, 0.0);
            rlVertex3f(x1, height, y1);


            rlTexCoord2f(0.0, 0.0);
            rlVertex3f(x1, height, y1);

            rlTexCoord2f(u, 1.0);
            rlVertex3f(x2, 0, y2);

            rlTexCoord2f(u, 0.0);
            rlVertex3f(x2, height, y2);
        }

        rlEnd();
    }
    shader.EndMode();
}

static void draw_world(Player *player, raylib::Window *window, const int scale) {
    static Fnt military_font("NOS/ENGINE/MILITARY.FNT");
    static Fnt military_small_font("NOS/ENGINE/MILITRY2.FNT");
    static Fnt military_large_font("NOS/ENGINE/MILITRY3.FNT");
    static Map game_map("NOS/ENGINE/MAPS/M0000.MAP");

    static uint64_t frame_count = 0;
    static const raylib::Color ground(0x79, 0x61, 0x3C, 0xFF);

    static const raylib::TextureUnmanaged *wall = TextureCache::LoadPot("NOS/ENGINE/TEXTURES/TAPEDRV0.POT");

    frame_count += 1;

    auto *camera3D = player->getCamera();

    player->update(frame_count);

    BeginDrawing();
    {
        window->ClearBackground(raylib::DARKGRAY);

        camera3D->BeginMode();
        {
            //DrawPlane(Vector3(0.0, 0.0, 0.0), Vector2(200, 200), ground);

            for (const auto &segment : game_map.getSegments()) {
                draw_wall(segment.x1, segment.y1, segment.x2, segment.y2, game_map.getTexture(segment.textureId));

                rlDrawRenderBatchActive();
            }
        }
        camera3D->EndMode();

//        military_font.write("1234567890  ABC", 20, 20, scale);
//        military_small_font.write("1234567890", 20, 100, scale);
//        military_large_font.write("1234567890", 20, 150, scale);
    }
    EndDrawing();
}


static void draw_title_screen(Player *player, raylib::Window *window, const int scale) {
    auto *title_screen = TextureCache::LoadPot("NOS/ENGINE/ART/NOS_LOGO.POT");

    BeginDrawing();
    {
        title_screen->Draw(raylib::Vector2(0, 0), 0, scale);
    }
    EndDrawing();
}

int main(int argc, char *argv[]) {
    cmdline::parser argparser;
    argparser.add<std::string>("datadir", 'd', "Data directory", false, "");
    argparser.add<int>("scale", 's', "render scale", false, 0);
    argparser.add<bool>("playback", 'p', "Disable music playback", false, false);
    argparser.parse_check(argc, argv);

    SetTraceLogLevel(LOG_WARNING);

    int scale = argparser.get<int>("scale");
    bool disable_music_playback = argparser.get<bool>("playback");
    std::string datadir = argparser.get<std::string>("datadir");

    const std::string title = "Nerves of Steel Remake " + std::string(" (v") + std::string(VERSION) + ")";

    if (!scale) {
        LaunchOptions launch_options(title, &scale, &disable_music_playback);
        if (!launch_options.run())
            exit(0);
    }

    if (datadir.size() == 0) {
        // Guess the path from location of NOS.BAT
        std::vector<std::filesystem::path> paths = {
            "./Nervesof",
            "./",
        };

        for (const auto &path : paths) {
            if (std::filesystem::exists(path / "NOS" / "NOS.BAT")) {
                datadir = path.string();
                break;
            }
        }
    }

    std::filesystem::current_path(datadir);

    SetConfigFlags(FLAG_MSAA_4X_HINT|FLAG_WINDOW_RESIZABLE);
    raylib::Window window(320*scale, 200*scale, title);
    SetTargetFPS(60);

//    window.SetExitKey(KEY_NULL);

//    raylib::AudioDevice audiodevice;

    raylib::Mouse::SetScale(1.0f/scale, 1.0f/scale);

    Player player;

    State old_state = player.getState();
    while (!window.ShouldClose()) {
        uint64_t player_input = player.getInput();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            player_input |= Input::PrimaryAction;
        } else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            player_input &= ~Input::PrimaryAction;
        }

        if (IsKeyPressed(KEY_W)) {
            player_input |= Input::StepForward;
        } else if (IsKeyReleased(KEY_W)) {
            player_input &= ~Input::StepForward;
        }

        if (IsKeyPressed(KEY_S)) {
            player_input |= Input::StepBack;
        } else if (IsKeyReleased(KEY_S)) {
            player_input &= ~Input::StepBack;
        }

        if (IsKeyPressed(KEY_A)) {
            player_input |= Input::StepLeft;
        } else if (IsKeyReleased(KEY_A)) {
            player_input &= ~Input::StepLeft;
        }

        if (IsKeyPressed(KEY_D)) {
            player_input |= Input::StepRight;
        } else if (IsKeyReleased(KEY_D)) {
            player_input &= ~Input::StepRight;
        }

        if (IsKeyPressed(KEY_UP)) {
            player_input |= Input::LookUp;
        } else if (IsKeyReleased(KEY_UP)) {
            player_input &= ~Input::LookUp;
        }

        if (IsKeyPressed(KEY_DOWN)) {
            player_input |= Input::LookDown;
        } else if (IsKeyReleased(KEY_DOWN)) {
            player_input &= ~Input::LookDown;
        }

        if (IsKeyPressed(KEY_LEFT)) {
            player_input |= Input::TurnLeft;
        } else if (IsKeyReleased(KEY_LEFT)) {
            player_input &= ~Input::TurnLeft;
        }

        if (IsKeyPressed(KEY_RIGHT)) {
            player_input |= Input::TurnRight;
        } else if (IsKeyReleased(KEY_RIGHT)) {
            player_input &= ~Input::TurnRight;
        }

        player.setInput(player_input);

        switch (player.getState()) {
            case State::World: 
                draw_world(&player, &window, scale);
                break;

            case State::Title:
                draw_title_screen(&player, &window, scale);
                break;

            case State::Exit:
                break;
        }
    }

    exit (0);
}
