#pragma once

#include "Mat.h"
#include "Vec.h"
#include "Timestamp.h"
#include "Utils.h"
#include "Reporter.h"
#include "Mouse.h"

namespace Crystal
{
	class Camera
	{
	public:
		Camera(Vec position, Vec lookingAt, Vec normal);
		Camera(Vec position, Vec lookingAt);
		Camera(Vec position);
		Camera();

		void setPosition(Vec position);
		void lookAt(Vec position);
		void lookAtNormal(Vec normal);
		void setCameraUp(Vec normal);
		void rotate(float x, float y);
		void enableFreeCam(bool option);
		void update(Timestamp timestamp);

		Vec getPosition();
		Vec getLookingAt();
		Vec getLookingNormal();
		Vec getNormal();

		Mat getMatrix();

	private:
		Vec position;
		Vec lookingAt;
		Vec normal;

		bool freeCam;
		float sensitivity;

		float rotX;
		float rotY;
	};
}