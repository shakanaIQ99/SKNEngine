#pragma once

class Vector3;

class Vector2
{
public:
	float x;
	float y;

public:
	//—ëƒxƒNƒgƒ‹¶¬
	Vector2();			
	//x,y¬•ª‚ğw’è‚µ‚Ä¶¬
	Vector2(float x, float y);	

	//ŠÖ”
	
	//’·‚³
	float length()const;				
	//³‹K‰»
	Vector2& normalize();		
	//³‹K‰»‚ğ“üè
	Vector2 getnormalize() const;
	//“àÏ
	float dot(const Vector2& v)const;	
	//ŠOÏ
	float cross(const Vector2& v)const;	

	//‹tƒxƒN
	Vector2 operator-()const;

	//”äŠr‘g

	bool operator==(const Vector2& v) const;
	bool operator!=(const Vector2& v) const;

	//‰‰Z‘g
	 
	//‘«‚µZ
	Vector2& operator+=(const Vector2& v);
	//ˆø‚«Z
	Vector2& operator-=(const Vector2& v);
	//Š|‚¯Z
	Vector2& operator*=(float s);
	//œZ
	Vector2& operator/=(float s);

	//‘«‚µZ
	Vector2 operator+(const Vector2& v) const;
	//ˆø‚«Z
	Vector2 operator-(const Vector2& v) const;
	//Š|‚¯Z
	Vector2 operator*(const float s) const;
	//œZ
	Vector2 operator/(const float s) const;

	operator Vector3() const;

};

Vector2 operator*(const float s, const Vector2& v);
