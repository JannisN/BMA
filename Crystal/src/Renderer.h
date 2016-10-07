#pragma once

#include "OpenGL.h"
#include "Camera.h"
#include "Mat.h"
#include "Reporter.h"
#include "Mat.h"
#include "Mesh.h"
#include "Shader.h"

namespace Crystal
{
	enum RenderMode
	{
		Perspective,
		Orthographic,
		TwoDimensional
	};

	class Mesh;

	class Renderer
	{
	public:
		Renderer();
		Renderer(int width, int height);
		Renderer(int width, int height, float fov);
		Renderer(int width, int height, RenderMode renderMode);
		Renderer(int width, int height, RenderMode renderMode, float fov);

		void update(Timestamp timestamp);

		void createObject(Mesh* mesh);

		void setUniformMatrix(Mat value, GLint shader, char* position);
		void setUniformMatrix(Mesh* mesh);

		void render(Mesh* mesh);

		void setCamera(Camera* camera);
		Camera* getCamera();
		Mat getViewProjectionMatrix();
		Mat getViewMatrix();
		Mat getProjectionMatrix();
		Mat getInverseProjectionMatrix();

		void setShader(Shader* shader);
		Shader* getShader();

		void setResolution(int width, int height);
		Vec2D getResolution();
		void setFov(float fov);

		void setRenderMode(RenderMode renderMode);
		void setClearColor(float red, float green, float blue);

		void clear();

	private:
		Camera* camera;
		Shader* shader;
		Mat projectionMatrix;
		int width;
		int height;
		float fov;
		RenderMode renderMode;

		float red;
		float green;
		float blue;

		void createVAO(Mesh* mesh);
		void createVBO(Mesh* mesh);

		void updateMatrix();
		void setPerspectiveProjection(float fov, float res, float fNear, float fFar);
		void setOrthoProjection(float left, float right, float bottom, float top);
	};
}