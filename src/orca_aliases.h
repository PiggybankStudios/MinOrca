/*
File:   orca_aliases.h
Author: Taylor Robbins
Date:   09\21\2024
Description:
	** In order to make Orca feel more like home, we are going to make aliases of
	** all the types and functions we want to use, in the naming convention I
	** normally employ on my other projects. This also serves as a handy reference
	** file for all the types and functions so I don't have to reference the actual
	** Orca source code every time I want to remember a function signature.
	** We'll also give this file the normal treatment we would do for a file in
	** gylib, where we have an Auto-Complete dictionary at the bottom, and there
	** are nice large headers that can be looked up and jumped to easily
	** (using the custom Sublime syntax that makes them into jump points)
*/

#ifndef _ORCA_ALIASES_H
#define _ORCA_ALIASES_H

// +--------------------------------------------------------------+
// |                         Basic Macros                         |
// +--------------------------------------------------------------+
//NOTE: Clang treats basic "inline" a bit different than I expected, "static inline" does what I want
#define INLINE static inline

// +--------------------------------------------------------------+
// |                            Types                             |
// +--------------------------------------------------------------+
// +==============================+
// |        Basic Typedefs        |
// +==============================+
// These types are already provided by Orca and match the names I give them normally:
//    u8, u16, u32, u64, i8, i16, i32, i64
typedef f32 r32;
typedef f64 r64;

// +==============================+
// |           Vectors            |
// +==============================+
typedef union
{
	oc_vec2 oc;
	r32 values[2];
	struct { r32 x, y; };
	struct { r32 width, height; };
} Vec2_t;
typedef Vec2_t v2;

typedef union
{
	oc_vec3 oc;
	r32 values[3];
	struct { r32 x, y, z; };
	struct { r32 width, height, depth; };
	struct { r32 r, g, b; };
	struct { r32 red, green, blue; };
} Vec3_t;
typedef Vec3_t v3;

typedef union
{
	oc_vec4 oc;
	r32 values[4];
	struct { r32 x, y, z, w; };
	struct { r32 width, height, depth, time; };
	struct { r32 r, g, b, a; };
	struct { r32 red, green, blue, alpha; };
} Vec4_t;
typedef Vec4_t v4;

typedef union
{
	oc_vec2i oc;
	i32 values[2];
	struct { i32 x, y; };
	struct { i32 width, height; };
} Vec2i_t;
typedef Vec2i_t v2i;

//TODO: Should we add v3i and v4i?

// +==============================+
// |           Matrices           |
// +==============================+
typedef union
{
	oc_mat2x3 oc;
	//         R  C
	r32 values[3][2]; //TODO: Is this the right order?
	struct { v3 row0; v3 row1; };
	struct
	{
		r32 r0c0, r0c1, r0c2;
		r32 r1c0, r1c1, r1c2;
	};
} Matrix2x3_t;
typedef Matrix2x3_t mat23;

//TODO: Should we add mat4? Or even mat3?

// +==============================+
// |          Rectangles          |
// +==============================+
typedef union
{
	oc_rect oc;
	union
	{
		v2 topLeft;
		struct { r32 x, y; };
		struct { r32 left, top; };
	};
	union
	{
		v2 size;
		struct { r32 width, height; };
	};
} Rectangle_t;
typedef Rectangle_t rec;

//TODO: Should we add reci, box? obb2? etc?

//TODO: Should we add Color_t and Colorf_t?

// +==============================+
// |     NewStruct Functions      |
// +==============================+
INLINE v2 NewVec2(r32 x, r32 y)
{
	v2 result;
	result.x = x;
	result.y = y;
	return result;
}
INLINE v3 NewVec3(r32 x, r32 y, r32 z)
{
	v3 result;
	result.x = x;
	result.y = y;
	result.z = z;
	return result;
}
INLINE v4 NewVec4(r32 x, r32 y, r32 z, r32 w)
{
	v4 result;
	result.x = x;
	result.y = y;
	result.z = z;
	result.w = w;
	return result;
}
INLINE v2i NewVec2i(i32 x, i32 y)
{
	v2i result;
	result.x = x;
	result.y = y;
	return result;
}
INLINE mat23 NewMat23(
	r32 r0c0, r32 r0c1, r32 r0c2,
	r32 r1c0, r32 r1c1, r32 r1c2)
{
	mat23 result;
	result.r0c0 = r0c0; result.r0c1 = r0c1; result.r0c2 = r0c2;
	result.r1c0 = r1c0; result.r1c1 = r1c1; result.r1c2 = r1c2;
	return result;
}
INLINE rec NewRec(r32 x, r32 y, r32 width, r32 height)
{
	rec result;
	result.x = x;
	result.y = y;
	result.width = width;
	result.height = height;
	return result;
}

// +===============================+
// | Operator Overload Equivalents |
// +===============================+
INLINE v2 Vec2Add(v2 left, v2 right)
{
	v2 result;
	result.x = left.x + right.x;
	result.y = left.y + right.y;
	return result;
}
INLINE v2 Vec2Subtract(v2 left, v2 right)
{
	v2 result;
	result.x = left.x - right.x;
	result.y = left.y - right.y;
	return result;
}
INLINE v2 Vec2Scale(v2 vector, r32 scalar)
{
	v2 result;
	result.x = vector.x * scalar;
	result.y = vector.y * scalar;
	return result;
}
//TODO: Vec2Multiply?
INLINE v2 Vec2Shrink(v2 vector, r32 divisor)
{
	v2 result;
	result.x = vector.x / divisor;
	result.y = vector.y / divisor;
	return result;
}
//TODO: Vec2Divide?
//TODO: Vec2BasicallyEqual?

// +==============================+
// |      Operator Overloads      |
// +==============================+
/*
v2 operator - (v2 vector)               { return NewVec2(-vector.x, -vector.y); }
v2 operator + (v2 left, v2 right)       { return Vec2Add(left, right); }
v2 operator - (v2 left, v2 right)       { return Vec2Subtract(left, right); }
v2 operator * (v2 vector, r32 scalar)   { return Vec2Scale(vector, scalar); }
v2 operator * (r32 scalar, v2 vector)   { return Vec2Scale(vector, scalar); }
v2 operator / (v2 vector, r32 scalar)   { return Vec2Shrink(vector, scalar); }
bool operator == (v2 left, v2 right)    { return (left.x == right.x && left.y == right.y); }
bool operator != (v2 left, v2 right)    { return (left.x != right.x || left.y != right.y); }
*/

#endif //  _ORCA_ALIASES_H

// +--------------------------------------------------------------+
// |                   Autocomplete Dictionary                    |
// +--------------------------------------------------------------+
/*
@Defines
INLINE
@Types
Vec2_t
v2
Vec3_t
v3
Vec4_t
v4
Vec2i_t
v2i
Vec3i_t
v3i
Matrix2x3_t
mat23
Rectangle_t
rec
@Functions
inline v2 NewVec2(r32 x, r32 y)
inline v3 NewVec3(r32 x, r32 y, r32 z)
inline v4 NewVec4(r32 x, r32 y, r32 z, r32 w)
inline v2i NewVec2i(i32 x, i32 y)
inline mat23 NewMat23(r32 r0c0, r32 r0c1, r32 r0c2, r32 r1c0, r32 r1c1, r32 r1c2)
inline rec NewRec(r32 x, r32 y, r32 width, r32 height)
*/
