/*!*********************************************************************
\file   SpriteAnimationFlags.h
\author a.nalpon@digipen.edu
\date   16-September-2023
\brief  this file contains the defined enumerated animation flags
for bitwise operations to query animation properties
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef SPRITE_ANIMATION_FLAGS_H
#define SPRITE_ANIMATION_FLAGS_H
#include <def.h>
namespace Graphics
{
  enum class SPRITE_ANIM_FLAGS : u32 {
    LOOPING         = 0x00000001, //0 - Looping the animation
    REWIND_COMPLETE = 0x00000002, //1 - Rewind animation on complete
    COMPLETED       = 0x00000004, //2 - INSTANCE FLAG: completed animation (useless for looping)
    //Bit3 = 0x00000008, // Bit 3
    //Bit4 = 0x00000010, // Bit 4
    //Bit5 = 0x00000020, // Bit 5
    //Bit6 = 0x00000040, // Bit 6
    //Bit7 = 0x00000080, // Bit 7
    //Bit8 = 0x00000100, // Bit 8
    //Bit9 = 0x00000200, // Bit 9
    //Bit10 = 0x00000400, // Bit 10
    //Bit11 = 0x00000800, // Bit 11
    //Bit12 = 0x00001000, // Bit 12
    //Bit13 = 0x00002000, // Bit 13
    //Bit14 = 0x00004000, // Bit 14
    //Bit15 = 0x00008000, // Bit 15
    //Bit16 = 0x00010000, // Bit 16
    //Bit17 = 0x00020000, // Bit 17
    //Bit18 = 0x00040000, // Bit 18
    //Bit19 = 0x00080000, // Bit 19
    //Bit20 = 0x00100000, // Bit 20
    //Bit21 = 0x00200000, // Bit 21
    //Bit22 = 0x00400000, // Bit 22
    //Bit23 = 0x00800000, // Bit 23
    //Bit24 = 0x01000000, // Bit 24
    //Bit25 = 0x02000000, // Bit 25
    //Bit26 = 0x04000000, // Bit 26
    //Bit27 = 0x08000000, // Bit 27
    //Bit28 = 0x10000000, // Bit 28
    //Bit29 = 0x20000000, // Bit 29
    //Bit30 = 0x40000000, // Bit 30
    //Bit31 = 0x80000000  // Bit 31
  };
  u32 operator|(u32 lhs, SPRITE_ANIM_FLAGS rhs);
  u32& operator|=(u32& lhs, SPRITE_ANIM_FLAGS rhs);
}
#endif