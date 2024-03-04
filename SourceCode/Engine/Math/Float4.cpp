#include "Float4.h"
#include "Vector2.h"
#include "Vector3.h"

Float4::operator Vector2() const
{
    return Vector2(x, y);
}

Float4::operator Vector3() const
{
    return Vector3(x, y, z);
}

Float4 Float4::operator/(const float& s) const
{
	Float4 temp = *this;
	temp.x /= s;
	temp.y /= s;
	temp.z /= s;
	temp.w /= s;
	return temp;
}

Float4& Float4::operator/=(const float& s)
{
	Float4 temp = *this / s;
	*this = temp;
	return *this;
}
