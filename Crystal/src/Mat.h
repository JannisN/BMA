#pragma once

#include "Vec.h"

#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/transform.hpp>
#include <GLM/mat4x4.hpp>
#include <GLM/vec4.hpp>
#include <GLM/gtc/type_ptr.hpp>

namespace Crystal
{
	class Mat
	{
	public:
		Mat();
		float* get();
		Mat getInverse();
		void inverse();

		static Mat perspective(float fov, float res, float fNear, float fFar);
		static Mat ortho(float left, float right, float bottom, float top);

		static Mat camera(Vec pos, Vec lookAt, Vec up);

		static Mat translate(Vec pos);
		static Mat scale(Vec size);
		static Mat scale(float size);
		static Mat rotate(Vec rot);
		static Mat rotate(Mat mat, Vec rot);

		Mat(glm::mat4 matrix);
		glm::mat4 matrix;
	};

	Mat operator * (Mat matrix1, Mat matrix2);
	Vec operator * (Mat matrix, Vec vector);
}