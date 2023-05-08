#pragma once

#include <SDL.h>
// #include <Defines/Defines.h>
#include <PCH/pch.h>

namespace Clunk
{
    enum class KeyCode : u8
    {
        #ifdef PLATFORM_SDL
            KC_BACKSPACE = SDLK_BACKSPACE,
            KC_TAB = SDLK_TAB,
            KC_ENTER = SDLK_RETURN,
            KC_ESCAPE = SDLK_ESCAPE,
            KC_SPACE = SDLK_SPACE,
            KC_EXCLAIM = SDLK_EXCLAIM,
            KC_DQUOTE = SDLK_QUOTEDBL,
            KC_HASH = SDLK_HASH,
            KC_DOLLAR = SDLK_DOLLAR,
            KC_PERCENT = SDLK_PERCENT,
            KC_AMPERSAND = SDLK_AMPERSAND,
            KC_SQUOTE = SDLK_QUOTE,
            KC_LPAREN = SDLK_LEFTPAREN,
            KC_RPAREN = SDLK_RIGHTPAREN,
            KC_ASTERISK = SDLK_ASTERISK,
            KC_PLUS = SDLK_PLUS,
            KC_COMMA = SDLK_COMMA,
            KC_MINUS = SDLK_MINUS,
            KC_PERIOD = SDLK_PERIOD,
            KC_SLASH = SDLK_SLASH,

            KC_0 = SDLK_0,
            KC_1 = SDLK_1,
            KC_2 = SDLK_2,
            KC_3 = SDLK_3,
            KC_4 = SDLK_4,
            KC_5 = SDLK_5,
            KC_6 = SDLK_6,
            KC_7 = SDLK_7,
            KC_8 = SDLK_8,
            KC_9 = SDLK_9,

            KC_COLON = SDLK_COLON,
            KC_SEMICOLON = SDLK_SEMICOLON,
            KC_LESS = SDLK_LESS,
            KC_EQUAL = SDLK_EQUALS,
            KC_GREATER = SDLK_GREATER,
            KC_QUESTION = SDLK_QUESTION,
            KC_AT = SDLK_AT,
            KC_LBRACK = SDLK_LEFTBRACKET,
            KC_BACKSLASH = SDLK_BACKSLASH,
            KC_RBRACK = SDLK_RIGHTBRACKET,
            KC_CARET = SDLK_CARET,
            KC_UNDERSCORE = SDLK_UNDERSCORE,
            KC_BACKQUOTE = SDLK_BACKQUOTE,

            KC_A = SDLK_a,
            KC_B = SDLK_b,
            KC_C = SDLK_c,
            KC_D = SDLK_d,
            KC_E = SDLK_e,
            KC_F = SDLK_f,
            KC_G = SDLK_g,
            KC_H = SDLK_h,
            KC_I = SDLK_i,
            KC_J = SDLK_j,
            KC_K = SDLK_k,
            KC_L = SDLK_l,
            KC_M = SDLK_m,
            KC_N = SDLK_n,
            KC_O = SDLK_o,
            KC_P = SDLK_p,
            KC_Q = SDLK_q,
            KC_R = SDLK_r,
            KC_S = SDLK_s,
            KC_T = SDLK_t,
            KC_U = SDLK_u,
            KC_V = SDLK_v,
            KC_W = SDLK_w,
            KC_X = SDLK_x,
            KC_Y = SDLK_y,
            KC_Z = SDLK_z,

            KC_DEL = SDLK_DELETE,
            KC_CAPSLOCK = static_cast<u8>(SDLK_CAPSLOCK),
            KC_RIGHT = static_cast<u8>(SDLK_RIGHT),
            KC_LEFT = static_cast<u8>(SDLK_LEFT),   
            KC_DOWN = static_cast<u8>(SDLK_DOWN),
            KC_UP = static_cast<u8>(SDLK_UP),
            KC_SUPER = static_cast<u8>(SDLK_MENU),

            KC_LCTRL = static_cast<u8>(SDLK_LCTRL),
            KC_LSHIFT = static_cast<u8>(SDLK_LSHIFT),
            KC_LALT = static_cast<u8>(SDLK_LALT),

            KC_RCTRL = static_cast<u8>(SDLK_RCTRL),
            KC_RSHIFT = static_cast<u8>(SDLK_RSHIFT),
            KC_RALT = static_cast<u8>(SDLK_RALT)
        #endif
        
    };

    KeyCode operator|(KeyCode& lhs, KeyCode& rhs)
    {
        return KeyCode(static_cast<u8>(lhs) | static_cast<u8>(rhs));
    }

    KeyCode operator&(KeyCode& lhs, KeyCode& rhs)
    {
        return KeyCode(static_cast<u8>(lhs) & static_cast<u8>(rhs));
    }
}