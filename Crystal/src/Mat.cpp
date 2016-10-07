#include "Mat.h"

namespace Crystal
{
	Mat::Mat()
	{
		matrix = glm::mat4(1.0f);
	}
	
	Mat::Mat(glm::mat4 matrix)
	{
		this->matrix = matrix;
	}

	float* Mat::get()
	{
		return glm::value_ptr(matrix);
	}

	Mat Mat::getInverse()
	{
		return Mat(glm::inverse(matrix));
	}

	void Mat::inverse()
	{
		matrix = glm::inverse(matrix);
	}

	Mat Mat::perspective(float fov, float res, float fNear, float fFar)
	{
		return Mat(glm::perspective(glm::radians(fov), res, fNear, fFar));
	}

	Mat Mat::ortho(float left, float right, float bottom, float top)
	{
		return Mat(glm::ortho(left, right, bottom, top));
	}

	Mat Mat::camera(Vec pos, Vec lookAt, Vec up)
	{
		return Mat(glm::lookAt(
			pos.vector,
			lookAt.vector,
			up.vector
			));
	}

	Mat Mat::translate(Vec pos)
	{
		return Mat(glm::translate(pos.vector));
	}

	Mat Mat::scale(Vec size)
	{
		return Mat(glm::scale(size.vector));
	}

	Mat Mat::scale(float size)
	{
		return Mat(glm::scale(Vec(size, size, size).vector));
	}

	Mat Mat::rotate(Vec rot)
	{
		return Mat(glm::rotate(glm::rotate(glm::rotate(glm::mat4(), rot.z(), glm::vec3(0, 0, 1)), rot.y(), glm::vec3(0, 1, 0)), rot.x(), glm::vec3(1, 0, 0)));
	}

	Mat Mat::rotate(Mat mat, Vec rot)
	{
		return Mat(glm::rotate(glm::rotate(glm::rotate(mat.matrix, rot.z(), glm::vec3(0, 0, 1)), rot.y(), glm::vec3(0, 1, 0)), rot.x(), glm::vec3(1, 0, 0)));
	}

	Mat operator * (Mat matrix1, Mat matrix2)
	{
		return Mat(matrix1.matrix * matrix2.matrix);
	}

	Vec operator * (Mat matrix, Vec vector)
	{
		return Vec(glm::vec3(matrix.matrix * glm::vec4(vector.vector, 1)));
	}
}