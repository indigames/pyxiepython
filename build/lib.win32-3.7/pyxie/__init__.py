"""
pyxie game engine
"""

from pyxie._pyxie import *

SAMPLERSTATE_WRAP = 0
SAMPLERSTATE_MIRROR = 1
SAMPLERSTATE_CLAMP = 2
SAMPLERSTATE_BORDER = 3
SAMPLERSTATE_LINEAR = 0
SAMPLERSTATE_NEAREST = 1
SAMPLERSTATE_NEAREST_MIPMAP_NEAREST = 2
SAMPLERSTATE_LINEAR_MIPMAP_NEAREST = 3
SAMPLERSTATE_NEAREST_MIPMAP_LINEAR = 4
SAMPLERSTATE_LINEAR_MIPMAP_LINEAR = 5

TARGET_PLATFORM_PC = 0
TARGET_PLATFORM_IOS = 1
TARGET_PLATFORM_ANDROID = 2
CURRENT_PLATFORM = getPlatform()

ANIMETION_PART_A = 0
ANIMETION_PART_B = 1
ANIMETION_PART_C = 2

ANIMETION_SLOT_A0 = 1
ANIMETION_SLOT_A1 = 2
ANIMETION_SLOT_B0 = 3
ANIMETION_SLOT_B1 = 4
ANIMETION_SLOT_C0 = 5
ANIMETION_SLOT_C1 = 6

ATTRIBUTE_ID_POSITION = 1
ATTRIBUTE_ID_NORMAL = 2
ATTRIBUTE_ID_TANGENT = 3
ATTRIBUTE_ID_BINORMAL = 4
ATTRIBUTE_ID_UV0 = 5
ATTRIBUTE_ID_UV1 = 6
ATTRIBUTE_ID_UV2 = 7
ATTRIBUTE_ID_UV3 = 8
ATTRIBUTE_ID_COLOR = 9
ATTRIBUTE_ID_BLENDINDICES = 10
ATTRIBUTE_ID_BLENDWEIGHT = 11
ATTRIBUTE_ID_PSIZE = 12

GL_BYTE            =0x1400
GL_UNSIGNED_BYTE   =0x1401
GL_SHORT           =0x1402
GL_UNSIGNED_SHORT  =0x1403
GL_INT             =0x1404
GL_UNSIGNED_INT    =0x1405
GL_FLOAT           =0x1406
GL_2_BYTES         =0x1407
GL_3_BYTES         =0x1408
GL_4_BYTES         =0x1409
GL_DOUBLE          =0x140A
GL_HALF_FLOAT      =0x140B

MAPCHANNEL_NONE = 0
MAPCHANNEL_COLOR_RED = 1
MAPCHANNEL_COLOR_ALPHA = 2
MAPCHANNEL_NORMAL_RED = 3
MAPCHANNEL_NORMAL_ALPHA = 4
MAPCHANNEL_LIGHT_RED = 5
MAPCHANNEL_LIGHT_ALPHA = 6
MAPCHANNEL_VERTEX_COLOR_RED = 7
MAPCHANNEL_VERTEX_COLOR_ALPHA = 8

AMBIENT_TYPE_NONE = 0
AMBIENT_TYPE_AMBIENT = 1
AMBIENT_TYPE_HEMISPHERE = 2
