#include "Camera.h"

namespace Crystal
{
	Camera::Camera(Vec position, Vec lookingAt, Vec normal)
	{
		this->position = position;
		this->lookingAt = lookingAt;
		this->normal = normal;

		this->rotX = 0;
		this->rotY = 0;

		freeCam = false;
		sensitivity = 0.2;
	}

	Camera::Camera(Vec position, Vec lookingAt) : Camera(position, lookingAt, Vec(0.0f, 1.0f, 0.0f))
	{
	}

	Camera::Camera(Vec position) : Camera(position, position + Vec(0.0f, 0.0f, -1.0f), Vec(0.0f, 1.0f, 0.0f))
	{
	}

	Camera::Camera() : Camera(Vec(0.0f, 0.0f, 0.0f), Vec(0.0f, 0.0f, -1.0f), Vec(0.0f, 1.0f, 0.0f))
	{
	}

	void Camera::setPosition(Vec position)
	{
		this->position = position;
	}

	void Camera::lookAt(Vec position)
	{
		this->lookingAt = position;
	}

	void Camera::lookAtNormal(Vec normal)
	{
		this->lookingAt = position + normal;
	}

	void Camera::setCameraUp(Vec normal)
	{
		this->normal = normal;
	}

	void Camera::rotate(float x, float y)
	{
		this->rotX = x;
		this->rotY = y;
	}

	void Camera::enableFreeCam(bool option)
	{
		freeCam = option;
	}

	void Camera::update(Timestamp timestamp)
	{
		if (freeCam)
		{
			this->rotX -= Mouse::getDeltaPos().y * timestamp.time() * sensitivity;
			this->rotY += Mouse::getDeltaPos().x * timestamp.time() * sensitivity;
			
			if (rotY >= 2 * pi)
				rotY = 0;
			if (rotY <= -2 * pi)
				rotY = 0;
			if (rotX > pi / 2 + 0.01)
				rotX = pi / 2 + 0.01;
			if (rotX < -pi / 2 )
				rotX = -pi / 2;
		}
	}

	Vec Camera::getPosition()
	{
		return position;
	}

	Vec Camera::getLookingAt()
	{
		return lookingAt;
	}

	Vec Camera::getLookingNormal()
	{
		return (lookingAt - position).getNormal();
	}

	Vec Camera::getNormal()
	{
		return normal;
	}

	Mat Camera::getMatrix()
	{
		//Reporter::report("loc: " + floatToString(rotY));
		//return Mat::camera(position, lookingAt, normal);
		return Mat::camera(position, Mat::rotate(Vec(rotX, rotY, 0)) * lookingAt, normal);
	}
}