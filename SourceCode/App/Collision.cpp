#include "Collision.h"

bool Collision::CheckSphere2Plane(const Sphere& sphere, const Plane& plane, Vector3* inter)
{
	float distV = sphere.center.dot(plane.normal);

	float dist = distV - plane.distance;

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

void Collision::ClosestPtPoint2Triangle(const Vector3& point, const Triangle& triangle, Vector3* closest)
{
	Vector3 p0_p1 = triangle.p1 - triangle.p0;
	Vector3 p0_p2 = triangle.p2 - triangle.p0;
	Vector3 p0_pt = point - triangle.p0;

	float d1 = p0_p1.dot(p0_pt);
	float d2 = p0_p2.dot(p0_pt);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		*closest = triangle.p0;
		return;
	}
	
	Vector3 p1_p0 = triangle.p0 - triangle.p1;
	Vector3 p1_p2 = triangle.p2 - triangle.p1;

	Vector3 p1_pt = point - triangle.p1;

	float d3 = p1_p0.dot(p1_pt);
	float d4 = p1_p2.dot(p1_pt);

	if (d3>= 0.0f && d4<= d3)
	{
		*closest = triangle.p1;
		return;
	}

	float vc = d1* d4 - d3 * d2;
	if (vc <= 0.0f && d1>= 0.0f && d3 <= 0.0f)
	{
		float v = d1/ (d1- d3);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}
	Vector3 p2_p0 = triangle.p0 - triangle.p2;
	Vector3 p2_p1 = triangle.p1 - triangle.p2;

	
	Vector3 p2_pt = point - triangle.p2;

	float d5 = p2_p0.dot(p2_pt);
	float d6 = p2_p1.dot(p2_pt);
	if (d6>= 0.0f && d5<= d6)
	{
		*closest = triangle.p2;
		return;
	}

	float vb = d5* d2- d1 * d6;
	if (vb <= 0.0f && d2>= 0.0f && d6<= 0.0f)
	{
		float w = d2/ (d2- d6);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}

	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;
}

bool Collision::CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter)
{
	Vector3 p;

	ClosestPtPoint2Triangle(sphere.center, triangle, &p);

	Vector3 v = p - sphere.center;

	float vf = v.dot(v);

	if (vf > sphere.radius * sphere.radius)
	{
		return false;
	}
	if (inter)
	{
		*inter = p;
	}


	return true;
}

bool Collision::CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance, Vector3* inter)
{
	const float epslion = 1.0e-5f;

	float d1 = plane.normal.dot(ray.dir);

	if (d1 > -epslion)
	{
		return false;
	}

	float d2 =plane.normal.dot(ray.start);

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

bool Collision::CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance, Vector3* inter)
{
	Plane plane;
	Vector3 interPlane;

	plane.normal = triangle.normal;

	plane.distance = triangle.normal.dot(triangle.p0);

	if (!CheckRay2Plane(ray, plane, distance, &interPlane))
	{
		return false;
	}

	const float epsilon = 1.0e-5f;
	Vector3 m;

	Vector3 pt_p0 = triangle.p0 - interPlane;
	Vector3 p0_p1 = triangle.p1 - triangle.p0;
	m = pt_p0.cross(p0_p1);
	if (m.dot(triangle.normal) < -epsilon)
	{
		return false;
	}
	Vector3 pt_p1 = triangle.p1 - interPlane;
	Vector3 p1_p2 = triangle.p2 - triangle.p1;
	m = pt_p1.cross(p1_p2);
	if (m.dot(triangle.normal) < -epsilon)
	{
		return false;
	}
	Vector3 pt_p2 = triangle.p2 - interPlane;
	Vector3 p2_p0 = triangle.p0 - triangle.p2;
	m = pt_p2.cross(p2_p0);
	if (m.dot(triangle.normal) < -epsilon)
	{
		return false;
	}

	if (inter)
	{
		*inter = interPlane;
	}


	return true;;
}

bool Collision::CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance, Vector3* inter)
{
	Vector3 m = ray.start - sphere.center;
	float b = m.dot( ray.dir);
	float c = m.dot( m) - sphere.radius * sphere.radius;

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

	Vector3 a = sphere2.center - sphere.center;

	


	if (a.length() < sphere.radius + sphere2.radius)
	{
		return true;
	}


	return false;
}
