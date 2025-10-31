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

#ifndef GAME_H
#define GAME_H

enum class State {
    Title,
    Exit,

    World,
};

enum class Collision {
    Pass,
    Block,
    Touch,
};

enum Input : uint64_t {
    PrimaryAction   = 1 << 0,
    StepForward     = 1 << 1,
    StepBack        = 1 << 2,
    StepLeft        = 1 << 3,
    StepRight       = 1 << 4,
    TurnLeft        = 1 << 5,
    TurnRight       = 1 << 6,
    LookUp          = 1 << 7,
    LookDown        = 1 << 8,
    Use             = 1 << 9,
};

#endif //GAME_H
