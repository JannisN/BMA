#include "Renderer.h"

namespace Crystal
{
	Renderer::Renderer(int width, int height, RenderMode renderMode, float fov)
	{
		this->width = width;
		this->height = height;
		this->fov = fov;
		setRenderMode(renderMode);
		setCamera(new Camera(Vec(0, 0, 0), Vec(0, 0, 1), Vec(0, 1, 0)));

		this->red = 100 / 255.0;
		this->green = 149 / 255.0;
		this->blue = 237 / 255.0;
	}

	void Renderer::update(Timestamp timestamp)
	{
		camera->update(timestamp);
	}

	Renderer::Renderer(int width, int height, float fov) : Renderer(width, height, Perspective, fov)
	{
	}

	Renderer::Renderer(int width, int height, RenderMode renderMode) : Renderer(width, height, renderMode, 90)
	{
	}

	Renderer::Renderer(int width, int height) : Renderer(width, height, Perspective, 90)
	{
	}

	Renderer::Renderer() : Renderer(1200, 720, Perspective, 90)
	{
	}

	void Renderer::createObject(Mesh* mesh)
	{
		createVAO(mesh);
		createVBO(mesh);
	}

	void Renderer::createVAO(Mesh* mesh)
	{
		glGenVertexArrays(1, &mesh->vao);
		Reporter::report("vao: " + floatToString(mesh->vao));
	}

	void Renderer::createVBO(Mesh* mesh)
	{
		//Reporter::report("xyz: " + floatToString(mesh->vertices.begin()->pos.x) + " " + floatToString(mesh->vertices.begin()->pos.y) + " " + floatToString(mesh->vertices.begin()->pos.z));

		shader->bind();


		Reporter::report("shader: " + floatToString(shader->id()));

		glBindVertexArray(mesh->vao);

		//Reporter::report("vbo: " + floatToString(vbo));

		std::list<float> posData;
		std::list<float> normalData;
		std::list<float> uvData;
		
		for (auto vertex : mesh->vertices)
		{
			posData.push_back(vertex.pos.x());
			posData.push_back(vertex.pos.y());
			posData.push_back(vertex.pos.z());

			normalData.push_back(vertex.normal.x());
			normalData.push_back(vertex.normal.y());
			normalData.push_back(vertex.normal.z());

			uvData.push_back(vertex.uvCoord.x());
			uvData.push_back(vertex.uvCoord.y());
		}

		std::vector<float> posVector(posData.begin(), posData.end());
		std::vector<float> normalVector(normalData.begin(), normalData.end());
		std::vector<float> uvVector(uvData.begin(), uvData.end());

		GLuint vboPosition;
		glGenBuffers(1, &vboPosition);
		glBindBuffer(GL_ARRAY_BUFFER, vboPosition);

		GLint location = glGetAttribLocation(shader->id(), "aPosition");
		if (!(location < 0))
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->vertices.size() * 3, &posVector[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
		Reporter::report("loc: " + floatToString(location));
		Reporter::report("size: " + floatToString(mesh->vertices.size() * 3));

		GLuint vboNormal;
		glGenBuffers(1, &vboNormal);
		glBindBuffer(GL_ARRAY_BUFFER, vboNormal);

		location = glGetAttribLocation(shader->id(), "aNormal");
		if (!(location < 0))
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->vertices.size() * 3, &normalVector[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
		Reporter::report("loc: " + floatToString(location));
		Reporter::report("size: " + floatToString(mesh->vertices.size() * 3));

		/*location = glGetAttribLocation(shader->id(), "aNormal");
		if (!(location < 0))
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->vertices.size() * 3, &normalVector[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
		Reporter::report("loc: " + floatToString(location));
		Reporter::report("size: " + floatToString(mesh->vertices.size() * 3));*/

		GLuint vboTextureCoord;
		glGenBuffers(1, &vboTextureCoord);
		glBindBuffer(GL_ARRAY_BUFFER, vboTextureCoord);

		location = glGetAttribLocation(shader->id(), "aTextureCoord");
		if (!(location < 0))
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->vertices.size() * 2, &uvVector[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, 0, 0);
		}
		Reporter::report("loc: " + floatToString(location));
	}

	void Renderer::setUniformMatrix(Mat value, GLint shader, char* position)
	{
		GLint location = glGetUniformLocation(shader, position);
		glUniformMatrix4fv(location, 1, GL_FALSE, value.get());
	}

	void Renderer::setUniformMatrix(Mesh* mesh)
	{
		GLint location = glGetUniformLocation(shader->id(), "uModelViewProjectionMatrix");
		glUniformMatrix4fv(location, 1, GL_FALSE, (getViewProjectionMatrix() * Mat::translate(mesh->pos) * Mat::rotate(mesh->rot) * Mat::scale(mesh->size)).get());

		location = glGetUniformLocation(shader->id(), "uModelViewMatrix");
		glUniformMatrix4fv(location, 1, GL_FALSE, (getViewMatrix() * Mat::translate(mesh->pos) * Mat::rotate(mesh->rot) * Mat::scale(mesh->size)).get());

		location = glGetUniformLocation(shader->id(), "uProjectionMatrix");
		glUniformMatrix4fv(location, 1, GL_FALSE, (getProjectionMatrix()).get());

		location = glGetUniformLocation(shader->id(), "uNormalMatrix");
		glUniformMatrix4fv(location, 1, GL_FALSE, (/*getViewMatrix() * Mat::translate(mesh->pos) */ Mat::rotate(mesh->rot) /* Mat::scale(mesh->size)*/).get());

		location = glGetUniformLocation(shader->id(), "uNormalScreenMatrix");
		glUniformMatrix4fv(location, 1, GL_FALSE, (/*getViewMatrix() * Mat::translate(mesh->pos) */getViewProjectionMatrix() * Mat::rotate(mesh->rot) /* Mat::scale(mesh->size)*/).get());

		glUniform1i(glGetUniformLocation(shader->id(), "texture1"), 0);
	}

	void Renderer::render(Mesh* mesh)
	{
		shader->bind();

		glBindVertexArray(mesh->vao);
		setUniformMatrix(mesh);
		glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.size());
	}

	void Renderer::setPerspectiveProjection(float fov, float res, float fNear, float fFar)
	{
		projectionMatrix = Mat::perspective(fov, res, fNear, fFar);
	}

	void Renderer::setOrthoProjection(float left, float right, float bottom, float top)
	{
		projectionMatrix = Mat::ortho(left, right, bottom, top);
	}

	void Renderer::setCamera(Camera* camera)
	{
		this->camera = camera;
	}

	Camera* Renderer::getCamera()
	{
		return camera;
	}

	Mat Renderer::getViewProjectionMatrix()
	{
		return projectionMatrix * camera->getMatrix();
	}

	Mat Renderer::getViewMatrix()
	{
		return camera->getMatrix();
	}

	Mat Renderer::getProjectionMatrix()
	{
		return projectionMatrix;
	}

	Mat Renderer::getInverseProjectionMatrix()
	{
		return projectionMatrix.getInverse();
	}

	void Renderer::setShader(Shader* shader)
	{
		this->shader = shader;
	}

	Shader* Renderer::getShader()
	{
		return shader;
	}

	void Renderer::setResolution(int width, int height)
	{
		if (width != 0 && height != 0)
		{
			this->width = width;
			this->height = height;

			glViewport(0, 0, width, height);

			glClearColor(red, green, blue, 1);

			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);

			updateMatrix();
		}
	}

	Vec2D Renderer::getResolution()
	{
		return Vec2D(width, height);
	}

	void Renderer::setFov(float fov)
	{
		this->fov = fov;
	}

	void Renderer::setRenderMode(RenderMode renderMode)
	{
		this->renderMode = renderMode;

		updateMatrix();
	}

	void Renderer::updateMatrix()
	{
		switch (this->renderMode)
		{
		case Perspective:
			setPerspectiveProjection(fov, (float)width / (float)height, 0.01, 10);
			break;
		case Orthographic:
		case TwoDimensional:
			setOrthoProjection((float)width / 2.0, (float)width / 2.0, (float)height / 2.0, (float)height / 2.0);
			break;
		}
	}

	void Renderer::setClearColor(float red, float green, float blue)
	{
		this->red = red;
		this->green = green;
		this->blue = blue;

		glClearColor(red, green, blue, 1);
	}

	void Renderer::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}