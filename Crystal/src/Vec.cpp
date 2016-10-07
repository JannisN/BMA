#include "Vec.h"

namespace Crystal
{
	Vec::Vec()
	{
		vector = glm::vec3(0.0f);
	}

	Vec::Vec(glm::vec3 vec)
	{
		vector = vec;
	}

	float Vec::x()
	{
		return vector.x;
	}

	float Vec::y()
	{
		return vector.y;
	}

	float Vec::z()
	{
		return vector.z;
	}

	Vec& Vec::operator = (Vec vector)
	{
		this->vector = vector.vector;
		/*this->x = this->vector.x;
		this->y = this->vector.y;
		this->z = this->vector.z;*/
		return *this;
		//return *this;
	}

	Vec::Vec(float x, float y, float z)
	{
		vector = glm::vec3(x, y, z); 
	}

	float* Vec::get()
	{
		return glm::value_ptr(vector);
	}

	Vec Vec::getNormal()
	{
		return Vec(glm::normalize(vector));
	}

	void Vec::normalize()
	{
		vector = glm::normalize(vector);
	}

	Vec operator + (Vec vector1, Vec vector2)
	{
		return Vec(vector1.vector + vector2.vector);
	}

	Vec operator - (Vec vector1, Vec vector2)
	{
		return Vec(vector1.vector - vector2.vector);
	}
}