#include "Collision.h"
using namespace DirectX;
bool Collision::CheckSphere2Plane(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter)
{
	XMVECTOR distV = XMVector3Dot(sphere.center, plane.normal);

	float dist = distV.m128_f32[0] - plane.distance;

	if (fabsf(dist) > sphere.radius)
	{
		return false;
	}

	if (inter)
	{
		*inter = -dist * plane.normal + sphere.center;
	}

	return true;
}

void Collision::ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point, const Triangle& triangle, DirectX::XMVECTOR* closest)
{
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	XMVECTOR p0_p2 = triangle.p2 - triangle.p0;
	XMVECTOR p0_pt = point - triangle.p0;

	XMVECTOR d1 = XMVector3Dot(p0_p1, p0_pt);
	XMVECTOR d2 = XMVector3Dot(p0_p2, p0_pt);

	if (d1.m128_f32[0] <= 0.0f && d2.m128_f32[0] <= 0.0f)
	{
		*closest = triangle.p0;
		return;
	}
	
	XMVECTOR p1_p0 = triangle.p0 - triangle.p1;
	XMVECTOR p1_p2 = triangle.p2 - triangle.p1;

	XMVECTOR p1_pt = point - triangle.p1;

	XMVECTOR d3 = XMVector3Dot(p1_p0, p1_pt);
	XMVECTOR d4 = XMVector3Dot(p1_p2, p1_pt);

	if (d3.m128_f32[0] >= 0.0f && d4.m128_f32[0] <= d3.m128_f32[0])
	{
		*closest = triangle.p1;
		return;
	}

	float vc = d1.m128_f32[0] * d4.m128_f32[0] - d3.m128_f32[0] * d2.m128_f32[0];
	if (vc <= 0.0f && d1.m128_f32[0] >= 0.0f && d3.m128_f32[0] <= 0.0f)
	{
		float v = d1.m128_f32[0] / (d1.m128_f32[0] - d3.m128_f32[0]);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}
	XMVECTOR p2_p0 = triangle.p0 - triangle.p2;
	XMVECTOR p2_p1 = triangle.p1 - triangle.p2;

	
	XMVECTOR p2_pt = point - triangle.p2;

	XMVECTOR d5 = XMVector3Dot(p2_p0, p2_pt);
	XMVECTOR d6 = XMVector3Dot(p2_p1, p2_pt);
	if (d6.m128_f32[0] >= 0.0f && d5.m128_f32[0] <= d6.m128_f32[0])
	{
		*closest = triangle.p2;
		return;
	}

	float vb = d5.m128_f32[0] * d2.m128_f32[0] - d1.m128_f32[0] * d6.m128_f32[0];
	if (vb <= 0.0f && d2.m128_f32[0] >= 0.0f && d6.m128_f32[0] <= 0.0f)
	{
		float w = d2.m128_f32[0] / (d2.m128_f32[0] - d6.m128_f32[0]);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}

	float va = d3.m128_f32[0] * d6.m128_f32[0] - d5.m128_f32[0] * d4.m128_f32[0];
	if (va <= 0.0f && (d4.m128_f32[0] - d3.m128_f32[0]) >= 0.0f && (d5.m128_f32[0] - d6.m128_f32[0]) >= 0.0f)
	{
		float w = (d4.m128_f32[0] - d3.m128_f32[0]) / ((d4.m128_f32[0] - d3.m128_f32[0]) + (d5.m128_f32[0] - d6.m128_f32[0]));
		*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;
}

bool Collision::CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter)
{
	XMVECTOR p;

	ClosestPtPoint2Triangle(sphere.center, triangle, &p);

	XMVECTOR v = p - sphere.center;

	v = XMVector3Dot(v, v);

	if (v.m128_f32[0] > sphere.radius * sphere.radius)
	{
		return false;
	}
	if (inter)
	{
		*inter = p;
	}


	return true;
}

bool Collision::CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance, DirectX::XMVECTOR* inter)
{
	const float epslion = 1.0e-5f;

	float d1 = XMVector3Dot(plane.normal, ray.dir).m128_f32[0];

	if (d1 > -epslion)
	{
		return false;
	}

	float d2 = XMVector3Dot(plane.normal, ray.start).m128_f32[0];

	float dist = d2 - plane.distance;

	float t = dist / -d1;

	if (t < 0)
	{
		return false;
	}

	if (distance)
	{
		*distance = t;
	}

	if (inter)
	{
		*inter = ray.start + t * ray.dir;
	}

	return true;
}

bool Collision::CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance, DirectX::XMVECTOR* inter)
{
	Plane plane;
	XMVECTOR interPlane;

	plane.normal = triangle.normal;

	plane.distance = XMVector3Dot(triangle.normal, triangle.p0).m128_f32[0];

	if (!CheckRay2Plane(ray, plane, distance, &interPlane))
	{
		return false;
	}

	const float epsilon = 1.0e-5f;
	XMVECTOR m;

	XMVECTOR pt_p0 = triangle.p0 - interPlane;
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	m = XMVector3Cross(pt_p0, p0_p1);
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon)
	{
		return false;
	}
	XMVECTOR pt_p1 = triangle.p1 - interPlane;
	XMVECTOR p1_p2 = triangle.p2 - triangle.p1;
	m = XMVector3Cross(pt_p1, p1_p2);
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon)
	{
		return false;
	}
	XMVECTOR pt_p2 = triangle.p2 - interPlane;
	XMVECTOR p2_p0 = triangle.p0 - triangle.p2;
	m = XMVector3Cross(pt_p2, p2_p0);
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon)
	{
		return false;
	}

	if (inter)
	{
		*inter = interPlane;
	}


	return true;;
}

bool Collision::CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance, DirectX::XMVECTOR* inter)
{
	XMVECTOR m = ray.start - sphere.center;
	float b = XMVector3Dot(m, ray.dir).m128_f32[0];
	float c = XMVector3Dot(m, m).m128_f32[0] - sphere.radius * sphere.radius;

	if (c > 0.0f && b > 0.0f)
	{
		return false;
	}

	float discr = b * b - c;

	if (discr < 0.0f)
	{
		return false;
	}

	float t = -b - sqrtf(discr);

	if (t < 0)
	{
		t = 0.0f;
	}
	if(distance)
	{
		*distance = t;
	}
	if (inter)
	{
		*inter = ray.start + t * ray.dir;
	}



	return true;
}

bool Collision::CheckSphereToSphere(const Sphere& sphere, const Sphere& sphere2)
{

	XMVECTOR dis = XMVector3Length(sphere2.center - sphere.center);

	float length = XMVectorGetX(dis);


	if (length < sphere.radius + sphere2.radius)
	{
		return true;
	}


	return false;
}
