#pragma once

#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/transform.hpp>
#include <GLM/mat4x4.hpp>
#include <GLM/vec4.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "Reporter.h"
#include "Utils.h"

namespace Crystal
{
	class Vec
	{
	public:
		Vec();
		Vec(float x, float y, float z);
		float* get();
		Vec getNormal();
		void normalize();
		Vec(glm::vec3 vector);
		glm::vec3 vector;
		float x();
		float y();
		float z();
		Vec& operator = (Vec vector);
	};

	Vec operator + (Vec vector1, Vec vector2);
	Vec operator - (Vec vector1, Vec vector2);
}