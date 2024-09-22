/*
File:   orca_addons.h
Author: Taylor Robbins
Date:   09\22\2024
Description:
	** After aliasing everything that exists in orca_aliases.h,
	** this file goes and adds new items that we want that Orca
	** does not provide. Many of the items in this file are
	** modified versions of things from GyLib
*/

#ifndef _ORCA_ADDONS_H
#define _ORCA_ADDONS_H

// +--------------------------------------------------------------+
// |                Vector and Rectangle Constants                |
// +--------------------------------------------------------------+
#define Vec2_Zero       NewVec2(0.0f, 0.0f)
#define Vec2_Zero_Const {0.0f, 0.0f}
#define Vec2_One        NewVec2( 1.0f,  1.0f)
#define Vec2_Half       NewVec2( 0.5f,  0.5f)
#define Vec2_Left       NewVec2(-1.0f,  0.0f)
#define Vec2_Right      NewVec2( 1.0f,  0.0f)
#define Vec2_Up         NewVec2( 0.0f, -1.0f)
#define Vec2_Down       NewVec2( 0.0f,  1.0f)

#define Vec3_Zero       NewVec3(0.0f, 0.0f, 0.0f)
#define Vec3_Zero_Const {0.0f, 0.0f, 0.0f}
#define Vec3_One        NewVec3( 1.0f,  1.0f,  1.0f)
#define Vec3_Half       NewVec3( 0.5f,  0.5f,  0.5f)
#define Vec3_Left       NewVec3(-1.0f,  0.0f,  0.0f)
#define Vec3_Right      NewVec3( 1.0f,  0.0f,  0.0f)
#define Vec3_Bottom     NewVec3( 0.0f, -1.0f,  0.0f)
#define Vec3_Top        NewVec3( 0.0f,  1.0f,  0.0f)
#define Vec3_Back       NewVec3( 0.0f,  0.0f, -1.0f)
#define Vec3_Front      NewVec3( 0.0f,  0.0f,  1.0f)
#define Vec3_Down       NewVec3( 0.0f, -1.0f,  0.0f)
#define Vec3_Up         NewVec3( 0.0f,  1.0f,  0.0f)
#define Vec3_Backward   NewVec3( 0.0f,  0.0f, -1.0f)
#define Vec3_Forward    NewVec3( 0.0f,  0.0f,  1.0f)

#define Vec4_Zero       NewVec4(0.0f, 0.0f, 0.0f, 0.0f)
#define Vec4_Zero_Const {0.0f, 0.0f, 0.0f, 0.0f}
#define Vec4_One        NewVec4( 1.0f,  1.0f,  1.0f, 1.0f)
#define Vec4_Half       NewVec4( 0.5f,  0.5f,  0.5f, 0.5f)
#define Vec4_Left       NewVec4(-1.0f,  0.0f,  0.0f, 0.0f)
#define Vec4_Right      NewVec4( 1.0f,  0.0f,  0.0f, 0.0f)
#define Vec4_Bottom     NewVec4( 0.0f, -1.0f,  0.0f, 0.0f)
#define Vec4_Top        NewVec4( 0.0f,  1.0f,  0.0f, 0.0f)
#define Vec4_Back       NewVec4( 0.0f,  0.0f, -1.0f, 0.0f)
#define Vec4_Front      NewVec4( 0.0f,  0.0f,  1.0f, 0.0f)
#define Vec4_Down       NewVec4( 0.0f, -1.0f,  0.0f, 0.0f)
#define Vec4_Up         NewVec4( 0.0f,  1.0f,  0.0f, 0.0f)
#define Vec4_Backward   NewVec4( 0.0f,  0.0f, -1.0f, 0.0f)
#define Vec4_Forward    NewVec4( 0.0f,  0.0f,  1.0f, 0.0f)
#define Vec4_Prevward   NewVec4( 0.0f,  0.0f,  0.0f,-1.0f)
#define Vec4_Nextward   NewVec4( 0.0f,  0.0f,  0.0f, 1.0f)

#define Vec2i_Zero       NewVec2i(0, 0)
#define Vec2i_Zero_Const {0, 0}
#define Vec2i_One        NewVec2i( 1,  1)
#define Vec2i_Left       NewVec2i(-1,  0)
#define Vec2i_Right      NewVec2i( 1,  0)
#define Vec2i_Up         NewVec2i( 0, -1)
#define Vec2i_Down       NewVec2i( 0,  1)

#define Rec_Zero           NewRec(0.0f, 0.0f, 0.0f, 0.0f)
#define Rec_Zero_Const     {0.0f, 0.0f, 0.0f, 0.0f}
#define Rec_Default        NewRec(0.0f, 0.0f, 1.0f, 1.0f)
#define Rec_Default_Const  { 0.0f, 0.0f, 1.0f, 1.0f }

// +--------------------------------------------------------------+
// |                 Vector / Rectangle Functions                 |
// +--------------------------------------------------------------+
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

INLINE v3 Vec3Add(v3 left, v3 right)
{
	v3 result;
	result.x = left.x + right.x;
	result.y = left.y + right.y;
	result.z = left.z + right.z;
	return result;
}
INLINE v3 Vec3Subtract(v3 left, v3 right)
{
	v3 result;
	result.x = left.x - right.x;
	result.y = left.y - right.y;
	result.z = left.z - right.z;
	return result;
}
INLINE v3 Vec3Scale(v3 vector, r32 scalar)
{
	v3 result;
	result.x = vector.x * scalar;
	result.y = vector.y * scalar;
	result.z = vector.z * scalar;
	return result;
}
//TODO: Vec3Multiply?
INLINE v3 Vec3Shrink(v3 vector, r32 divisor)
{
	v3 result;
	result.x = vector.x / divisor;
	result.y = vector.y / divisor;
	result.z = vector.z / divisor;
	return result;
}
//TODO: Vec3Divide?
//TODO: Vec3BasicallyEqual?

INLINE v4 Vec4Add(v4 left, v4 right)
{
	v4 result;
	result.x = left.x + right.x;
	result.y = left.y + right.y;
	result.z = left.z + right.z;
	result.w = left.w + right.w;
	return result;
}
INLINE v4 Vec4Subtract(v4 left, v4 right)
{
	v4 result;
	result.x = left.x - right.x;
	result.y = left.y - right.y;
	result.z = left.z - right.z;
	result.w = left.w - right.w;
	return result;
}
INLINE v4 Vec4Scale(v4 vector, r32 scalar)
{
	v4 result;
	result.x = vector.x * scalar;
	result.y = vector.y * scalar;
	result.z = vector.z * scalar;
	result.w = vector.w * scalar;
	return result;
}
//TODO: Vec4Multiply?
INLINE v4 Vec4Shrink(v4 vector, r32 divisor)
{
	v4 result;
	result.x = vector.x / divisor;
	result.y = vector.y / divisor;
	result.z = vector.z / divisor;
	result.w = vector.w / divisor;
	return result;
}
//TODO: Vec4Divide?
//TODO: Vec4BasicallyEqual?

INLINE v2i Vec2iAdd(v2i left, v2i right)
{
	v2i result;
	result.x = left.x + right.x;
	result.y = left.y + right.y;
	return result;
}
INLINE v2i Vec2iSubtract(v2i left, v2i right)
{
	v2i result;
	result.x = left.x - right.x;
	result.y = left.y - right.y;
	return result;
}
INLINE v2i Vec2iScale(v2i vector, i32 scalar)
{
	v2i result;
	result.x = vector.x * scalar;
	result.y = vector.y * scalar;
	return result;
}
//TODO: Vec2iMultiply?
INLINE v2i Vec2iShrink(v2i vector, i32 divisor)
{
	v2i result;
	result.x = vector.x / divisor;
	result.y = vector.y / divisor;
	return result;
}
//TODO: Vec2iDivide?
//TODO: Vec2iBasicallyEqual?

//TODO: RecShift(x, y)?
INLINE rec RecShift(rec rectangle, v2 amount)
{
	rec result;
	result.x = rectangle.x + amount.x;
	result.y = rectangle.y + amount.y;
	result.size = rectangle.size;
	return result;
}
INLINE rec RecScale(rec rectangle, r32 scalar)
{
	rec result;
	result.x = rectangle.x * scalar;
	result.y = rectangle.y * scalar;
	result.width = rectangle.width * scalar;
	result.height = rectangle.height * scalar;
	return result;
}
//TODO: RecScale2/
//TODO: RecBasicallyEqual?

// +==============================+
// |      Operator Overloads      |
// +==============================+
INLINE v2 operator - (v2 vector)                            { return NewVec2(-vector.x, -vector.y); }
INLINE v2 operator + (v2 left, v2 right)                    { return Vec2Add(left, right); }
INLINE v2 operator - (v2 left, v2 right)                    { return Vec2Subtract(left, right); }
INLINE v2 operator * (v2 vector, r32 scalar)                { return Vec2Scale(vector, scalar); }
INLINE v2 operator * (r32 scalar, v2 vector)                { return Vec2Scale(vector, scalar); }
INLINE v2 operator / (v2 vector, r32 scalar)                { return Vec2Shrink(vector, scalar); }
INLINE bool operator == (v2 left, v2 right)                 { return (left.x == right.x && left.y == right.y); }
INLINE bool operator != (v2 left, v2 right)                 { return (left.x != right.x || left.y != right.y); }
INLINE void operator += (v2& leftSide, const v2& rightSide) { leftSide.x += rightSide.x; leftSide.y += rightSide.y; }
INLINE void operator -= (v2& leftSide, const v2& rightSide) { leftSide.x -= rightSide.x; leftSide.y -= rightSide.y; }
INLINE void operator *= (v2& leftSide, const r32 rightSide) { leftSide.x *= rightSide; leftSide.y *= rightSide; }
INLINE void operator /= (v2& leftSide, const r32 rightSide) { leftSide.x /= rightSide; leftSide.y /= rightSide; }

INLINE v3 operator - (v3 vector)                            { return NewVec3(-vector.x, -vector.y, -vector.z); }
INLINE v3 operator + (v3 left, v3 right)                    { return Vec3Add(left, right); }
INLINE v3 operator - (v3 left, v3 right)                    { return Vec3Subtract(left, right); }
INLINE v3 operator * (v3 vector, r32 scalar)                { return Vec3Scale(vector, scalar); }
INLINE v3 operator * (r32 scalar, v3 vector)                { return Vec3Scale(vector, scalar); }
INLINE v3 operator / (v3 vector, r32 scalar)                { return Vec3Shrink(vector, scalar); }
INLINE bool operator == (v3 left, v3 right)                 { return (left.x == right.x && left.y == right.y && left.z == right.z); }
INLINE bool operator != (v3 left, v3 right)                 { return (left.x != right.x || left.y != right.y || left.z != right.z); }
INLINE void operator += (v3& leftSide, const v3& rightSide) { leftSide.x += rightSide.x; leftSide.y += rightSide.y; leftSide.z += rightSide.z; }
INLINE void operator -= (v3& leftSide, const v3& rightSide) { leftSide.x -= rightSide.x; leftSide.y -= rightSide.y; leftSide.z -= rightSide.z; }
INLINE void operator *= (v3& leftSide, const r32 rightSide) { leftSide.x *= rightSide; leftSide.y *= rightSide; leftSide.z *= rightSide; }
INLINE void operator /= (v3& leftSide, const r32 rightSide) { leftSide.x /= rightSide; leftSide.y /= rightSide; leftSide.z /= rightSide; }

INLINE v4 operator - (v4 vector)                            { return NewVec4(-vector.x, -vector.y, -vector.z, -vector.w); }
INLINE v4 operator + (v4 left, v4 right)                    { return Vec4Add(left, right); }
INLINE v4 operator - (v4 left, v4 right)                    { return Vec4Subtract(left, right); }
INLINE v4 operator * (v4 vector, r32 scalar)                { return Vec4Scale(vector, scalar); }
INLINE v4 operator * (r32 scalar, v4 vector)                { return Vec4Scale(vector, scalar); }
INLINE v4 operator / (v4 vector, r32 scalar)                { return Vec4Shrink(vector, scalar); }
INLINE bool operator == (v4 left, v4 right)                 { return (left.x == right.x && left.y == right.y && left.z == right.z && left.w == right.w); }
INLINE bool operator != (v4 left, v4 right)                 { return (left.x != right.x || left.y != right.y || left.z != right.z || left.w != right.w); }
INLINE void operator += (v4& leftSide, const v4& rightSide) { leftSide.x += rightSide.x; leftSide.y += rightSide.y; leftSide.z += rightSide.z; leftSide.w += rightSide.w; }
INLINE void operator -= (v4& leftSide, const v4& rightSide) { leftSide.x -= rightSide.x; leftSide.y -= rightSide.y; leftSide.z -= rightSide.z; leftSide.w -= rightSide.w; }
INLINE void operator *= (v4& leftSide, const r32 rightSide) { leftSide.x *= rightSide; leftSide.y *= rightSide; leftSide.z *= rightSide; leftSide.w *= rightSide; }
INLINE void operator /= (v4& leftSide, const r32 rightSide) { leftSide.x /= rightSide; leftSide.y /= rightSide; leftSide.z /= rightSide; leftSide.w /= rightSide; }

INLINE v2i operator - (v2i vector)                            { return NewVec2i(-vector.x, -vector.y); }
INLINE v2i operator + (v2i left, v2i right)                   { return Vec2iAdd(left, right); }
INLINE v2i operator - (v2i left, v2i right)                   { return Vec2iSubtract(left, right); }
INLINE v2i operator * (v2i vector, i32 scalar)                { return Vec2iScale(vector, scalar); }
INLINE v2i operator * (i32 scalar, v2i vector)                { return Vec2iScale(vector, scalar); }
INLINE v2i operator / (v2i vector, i32 scalar)                { return Vec2iShrink(vector, scalar); }
INLINE bool operator == (v2i left, v2i right)                 { return (left.x == right.x && left.y == right.y); }
INLINE bool operator != (v2i left, v2i right)                 { return (left.x != right.x || left.y != right.y); }
INLINE void operator += (v2i& leftSide, const v2i& rightSide) { leftSide.x += rightSide.x; leftSide.y += rightSide.y; }
INLINE void operator -= (v2i& leftSide, const v2i& rightSide) { leftSide.x -= rightSide.x; leftSide.y -= rightSide.y; }
INLINE void operator *= (v2i& leftSide, const i32 rightSide)  { leftSide.x *= rightSide; leftSide.y *= rightSide; }
INLINE void operator /= (v2i& leftSide, const i32 rightSide)  { leftSide.x /= rightSide; leftSide.y /= rightSide; }

INLINE bool operator == (rec left, rec right)             { return  (left.x == right.x && left.y == right.y && left.width == right.width && left.height == right.height); }
INLINE bool operator != (rec left, rec right)             { return !(left.x == right.x && left.y == right.y && left.width == right.width && left.height == right.height); }
INLINE rec operator + (rec rectangle, v2 vector)          { return RecShift(rectangle, vector); }
INLINE rec operator - (rec rectangle, v2 vector)          { return RecShift(rectangle, -vector); }
INLINE rec operator * (rec rectangle, r32 scale)          { return RecScale(rectangle, scale); }
INLINE rec operator / (rec rectangle, r32 scale)          { return RecScale(rectangle, 1/scale); }
INLINE void operator += (rec& leftSide, const v2& vector) { leftSide.topLeft += vector; }
INLINE void operator -= (rec& leftSide, const v2& vector) { leftSide.topLeft -= vector; }

#endif //  _ORCA_ADDONS_H

// +--------------------------------------------------------------+
// |                   Autocomplete Dictionary                    |
// +--------------------------------------------------------------+
/*
@Defines
Vec2_Zero
Vec2_Zero_Const
Vec2_One
Vec2_Half
Vec2_Left
Vec2_Right
Vec2_Up
Vec2_Down
Vec3_Zero
Vec3_Zero_Const
Vec3_One
Vec3_Half
Vec3_Left
Vec3_Right
Vec3_Bottom
Vec3_Top
Vec3_Back
Vec3_Front
Vec3_Down
Vec3_Up
Vec3_Backward
Vec3_Forward
Vec4_Zero
Vec4_Zero_Const
Vec4_Zero
Vec4_One
Vec4_Half
Vec4_Left
Vec4_Right
Vec4_Bottom
Vec4_Top
Vec4_Back
Vec4_Front
Vec4_Down
Vec4_Up
Vec4_Backward
Vec4_Forward
Vec4_Prevward
Vec4_Nextward
Vec2i_Zero
Vec2i_Zero_Const
Vec2i_One
Vec2i_Left
Vec2i_Right
Vec2i_Up
Vec2i_Down
Rec_Zero
Rec_Zero_Const
Rec_Default
Rec_Default_Const
@Types
@Functions
INLINE v2 Vec2Add(v2 left, v2 right)
INLINE v2 Vec2Subtract(v2 left, v2 right)
INLINE v2 Vec2Scale(v2 vector, r32 scalar)
INLINE v2 Vec2Shrink(v2 vector, r32 divisor)
INLINE v3 Vec3Add(v3 left, v3 right)
INLINE v3 Vec3Subtract(v3 left, v3 right)
INLINE v3 Vec3Scale(v3 vector, r32 scalar)
INLINE v3 Vec3Shrink(v3 vector, r32 divisor)
INLINE v4 Vec4Add(v4 left, v4 right)
INLINE v4 Vec4Subtract(v4 left, v4 right)
INLINE v4 Vec4Scale(v4 vector, r32 scalar)
INLINE v4 Vec4Shrink(v4 vector, r32 divisor)
INLINE v2i Vec2iAdd(v2i left, v2i right)
INLINE v2i Vec2iSubtract(v2i left, v2i right)
INLINE v2i Vec2iScale(v2i vector, i32 scalar)
INLINE v2i Vec2iShrink(v2i vector, i32 divisor)
INLINE rec RecShift(rec rectangle, v2 amount)
INLINE rec RecScale(rec rectangle, r32 scalar)
*/