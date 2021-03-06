﻿#pragma once
#include <cstdint>
#include "ui_color.h"

// ui
namespace LongUI {
    // rgba
    constexpr uint32_t operator ""_rgba(unsigned long long value) noexcept {
        static_assert(sizeof(value) >= sizeof(uint32_t), "bad sizeof");
        return helper::rgba(static_cast<uint32_t>(value));
    }
    // rgba color
    enum ColorRGBA : uint32_t {
        // SP
        RGBA_TianyiBlue     = 0x66CCFFFF_rgba,
        RGBA_Transparent    = 0x00000000_rgba,
        RGBA_TransparentB   = 0x00000000_rgba, // BLACK
        RGBA_TransparentM   = 0x80808000_rgba, // MIDDLE
        RGBA_TransparentW   = 0xFFFFFF00_rgba, // WHITE
        // WEB#141
        RGBA_RebeccaPurple  = 0x663399FF_rgba,
        // WEB#140
        RGBA_AliceBlue      = 0xF0F8FFFF_rgba,
        RGBA_AntiqueWhite   = 0xFAEBD7FF_rgba,
        RGBA_Aqua           = 0x00FFFFFF_rgba,
        RGBA_Aquamarine     = 0x7FFFD4FF_rgba,
        RGBA_Azure          = 0xF0FFFFFF_rgba,
        RGBA_Beige          = 0xF5F5DCFF_rgba,
        RGBA_Bisque         = 0xFFE4C4FF_rgba,
        RGBA_Black          = 0x000000FF_rgba,
        RGBA_BlanchedAlmond = 0xFFEBCDFF_rgba,
        RGBA_Blue           = 0x0000FFFF_rgba,
        RGBA_BlueViolet     = 0x8A2BE2FF_rgba,
        RGBA_Brown          = 0xA52A2AFF_rgba,
        RGBA_BurlyWood      = 0xDEB887FF_rgba,
        RGBA_CadetBlue      = 0x5F9EA0FF_rgba,
        RGBA_Chartreuse     = 0x7FFF00FF_rgba,
        RGBA_Chocolate      = 0xD2691EFF_rgba,
        RGBA_Coral          = 0xFF7F50FF_rgba,
        RGBA_CornflowerBlue = 0x6495EDFF_rgba,
        RGBA_Cornsilk       = 0xFFF8DCFF_rgba,
        RGBA_Crimson        = 0xDC143CFF_rgba,
        RGBA_Cyan           = 0x00FFFFFF_rgba,
        RGBA_DarkBlue       = 0x00008BFF_rgba,
        RGBA_DarkCyan       = 0x008B8BFF_rgba,
        RGBA_DarkGoldenRod  = 0xB8860BFF_rgba,
        RGBA_DarkGray       = 0xA9A9A9FF_rgba,
        RGBA_DarkGreen      = 0x006400FF_rgba,
        RGBA_DarkKhaki      = 0xBDB76BFF_rgba,
        RGBA_DarkMagenta    = 0x8B008BFF_rgba,
        RGBA_DarkOliveGreen = 0x556B2FFF_rgba,
        RGBA_Darkorange     = 0xFF8C00FF_rgba,
        RGBA_DarkOrchid     = 0x9932CCFF_rgba,
        RGBA_DarkRed        = 0x8B0000FF_rgba,
        RGBA_DarkSalmon     = 0xE9967AFF_rgba,
        RGBA_DarkSeaGreen   = 0x8FBC8FFF_rgba,
        RGBA_DarkSlateBlue  = 0x483D8BFF_rgba,
        RGBA_DarkSlateGray  = 0x2F4F4FFF_rgba,
        RGBA_DarkTurquoise  = 0x00CED1FF_rgba,
        RGBA_DarkViolet     = 0x9400D3FF_rgba,
        RGBA_DeepPink       = 0xFF1493FF_rgba,
        RGBA_DeepSkyBlue    = 0x00BFFFFF_rgba,
        RGBA_DimGray        = 0x696969FF_rgba,
        RGBA_DimGrey        = 0x696969FF_rgba,
        RGBA_DodgerBlue     = 0x1E90FFFF_rgba,
        RGBA_FireBrick      = 0xB22222FF_rgba,
        RGBA_FloralWhite    = 0xFFFAF0FF_rgba,
        RGBA_ForestGreen    = 0x228B22FF_rgba,
        RGBA_Fuchsia        = 0xFF00FFFF_rgba,
        RGBA_Gainsboro      = 0xDCDCDCFF_rgba,
        RGBA_GhostWhite     = 0xF8F8FFFF_rgba,
        RGBA_Gold           = 0xFFD700FF_rgba,
        RGBA_GoldenRod      = 0xDAA520FF_rgba,
        RGBA_Gray           = 0x808080FF_rgba,
        RGBA_Green          = 0x008000FF_rgba,
        RGBA_GreenYellow    = 0xADFF2FFF_rgba,
        RGBA_HoneyDew       = 0xF0FFF0FF_rgba,
        RGBA_HotPink        = 0xFF69B4FF_rgba,
        RGBA_IndianRed      = 0xCD5C5CFF_rgba,
        RGBA_Indigo         = 0x4B0082FF_rgba,
        RGBA_Ivory          = 0xFFFFF0FF_rgba,
        RGBA_Khaki          = 0xF0E68CFF_rgba,
        RGBA_Lavender       = 0xE6E6FAFF_rgba,
        RGBA_LavenderBlush  = 0xFFF0F5FF_rgba,
        RGBA_LawnGreen      = 0x7CFC00FF_rgba,
        RGBA_LemonChiffon   = 0xFFFACDFF_rgba,
        RGBA_LightBlue      = 0xADD8E6FF_rgba,
        RGBA_LightCoral     = 0xF08080FF_rgba,
        RGBA_LightCyan      = 0xE0FFFFFF_rgba,
        RGBA_LightGoldenRodYellow   = 0xFAFAD2FF_rgba,
        RGBA_LightGray      = 0xD3D3D3FF_rgba,
        RGBA_LightGreen     = 0x90EE90FF_rgba,
        RGBA_LightPink      = 0xFFB6C1FF_rgba,
        RGBA_LightSalmon    = 0xFFA07AFF_rgba,
        RGBA_LightSeaGreen  = 0x20B2AAFF_rgba,
        RGBA_LightSkyBlue   = 0x87CEFAFF_rgba,
        RGBA_LightSlateGray = 0x778899FF_rgba,
        RGBA_LightSteelBlue = 0xB0C4DEFF_rgba,
        RGBA_LightYellow    = 0xFFFFE0FF_rgba,
        RGBA_Lime           = 0x00FF00FF_rgba,
        RGBA_LimeGreen      = 0x32CD32FF_rgba,
        RGBA_Linen          = 0xFAF0E6FF_rgba,
        RGBA_Magenta        = 0xFF00FFFF_rgba,
        RGBA_Maroon         = 0x800000FF_rgba,
        RGBA_MediumAquaMarine       = 0x66CDAAFF_rgba,
        RGBA_MediumBlue     = 0x0000CDFF_rgba,
        RGBA_MediumOrchid   = 0xBA55D3FF_rgba,
        RGBA_MediumPurple   = 0x9370DBFF_rgba,
        RGBA_MediumSeaGreen = 0x3CB371FF_rgba,
        RGBA_MediumSlateBlue= 0x7B68EEFF_rgba,
        RGBA_MediumSpringGreen      = 0x00FA9AFF_rgba,
        RGBA_MediumTurquoise= 0x48D1CCFF_rgba,
        RGBA_MediumVioletRed= 0xC71585FF_rgba,
        RGBA_MidnightBlue   = 0x191970FF_rgba,
        RGBA_MintCream      = 0xF5FFFAFF_rgba,
        RGBA_MistyRose      = 0xFFE4E1FF_rgba,
        RGBA_Moccasin       = 0xFFE4B5FF_rgba,
        RGBA_NavajoWhite    = 0xFFDEADFF_rgba,
        RGBA_Navy           = 0x000080FF_rgba,
        RGBA_OldLace        = 0xFDF5E6FF_rgba,
        RGBA_Olive          = 0x808000FF_rgba,
        RGBA_OliveDrab      = 0x6B8E23FF_rgba,
        RGBA_Orange         = 0xFFA500FF_rgba,
        RGBA_OrangeRed      = 0xFF4500FF_rgba,
        RGBA_Orchid         = 0xDA70D6FF_rgba,
        RGBA_PaleGoldenRod  = 0xEEE8AAFF_rgba,
        RGBA_PaleGreen      = 0x98FB98FF_rgba,
        RGBA_PaleTurquoise  = 0xAFEEEEFF_rgba,
        RGBA_PaleVioletRed  = 0xDB7093FF_rgba,
        RGBA_PapayaWhip     = 0xFFEFD5FF_rgba,
        RGBA_PeachPuff      = 0xFFDAB9FF_rgba,
        RGBA_Peru           = 0xCD853FFF_rgba,
        RGBA_Pink           = 0xFFC0CBFF_rgba,
        RGBA_Plum           = 0xDDA0DDFF_rgba,
        RGBA_PowderBlue     = 0xB0E0E6FF_rgba,
        RGBA_Purple         = 0x800080FF_rgba,
        RGBA_Red            = 0xFF0000FF_rgba,
        RGBA_RosyBrown      = 0xBC8F8FFF_rgba,
        RGBA_RoyalBlue      = 0x4169E1FF_rgba,
        RGBA_SaddleBrown    = 0x8B4513FF_rgba,
        RGBA_Salmon         = 0xFA8072FF_rgba,
        RGBA_SandyBrown     = 0xF4A460FF_rgba,
        RGBA_SeaGreen       = 0x2E8B57FF_rgba,
        RGBA_SeaShell       = 0xFFF5EEFF_rgba,
        RGBA_Sienna         = 0xA0522DFF_rgba,
        RGBA_Silver         = 0xC0C0C0FF_rgba,
        RGBA_SkyBlue        = 0x87CEEBFF_rgba,
        RGBA_SlateBlue      = 0x6A5ACDFF_rgba,
        RGBA_SlateGray      = 0x708090FF_rgba,
        RGBA_Snow           = 0xFFFAFAFF_rgba,
        RGBA_SpringGreen    = 0x00FF7FFF_rgba,
        RGBA_SteelBlue      = 0x4682B4FF_rgba,
        RGBA_Tan            = 0xD2B48CFF_rgba,
        RGBA_Teal           = 0x008080FF_rgba,
        RGBA_Thistle        = 0xD8BFD8FF_rgba,
        RGBA_Tomato         = 0xFF6347FF_rgba,
        RGBA_Turquoise      = 0x40E0D0FF_rgba,
        RGBA_Violet         = 0xEE82EEFF_rgba,
        RGBA_Wheat          = 0xF5DEB3FF_rgba,
        RGBA_White          = 0xFFFFFFFF_rgba,
        RGBA_WhiteSmoke     = 0xF5F5F5FF_rgba,
        RGBA_Yellow         = 0xFFFF00FF_rgba,
        RGBA_YellowGreen    = 0x9ACD32FF_rgba,
    };
}