#include "RenderScene.h"

namespace Crystal
{
	void RenderScene::init(Renderer* renderer)
	{
		/*const std::string vertexShader =
			"#version 400\n"
			"in vec3 vp;"
			"uniform mat4 MVP;"
			"void main () {"
			"  gl_Position = MVP * vec4 (vp, 1.0);"
			"}";

		const std::string fragmentShader =
			"#version 400\n"
			"out vec4 frag_colour;"
			"void main () {"
			"  frag_colour = vec4(1.0, 1.0, 1.0, 1.0);"
			"}";

		Shader shader(vertexShader, fragmentShader);
		shader.bind();

		GLfloat vertexData[] = {
			//  X     Y     Z       U     V
			0.5f, 0.5f, 0.5f,//   0.5f, 1.0f,
			-0.5f, 0.5f, 0.5f,
			0.5f,-0.5f, 0.5f,//   0.0f, 0.0f,,//,   1.0f, 0.0f,
			-0.5f,-0.5f, 0.5f,
			0.5f,-0.5f, 0.5f,//   0.0f, 0.0f,
			-0.5f, 0.5f, 0.5f
		};

		/*vao = renderer->createVAO();
		renderer->createVBO(vao, vertexData, 18, shader.id(), "vp");*/
		glClearColor(100 / 255.0, 149 / 255.0, 237 / 255.0, 1);
		//Mouse::setWrap(true);*/


		const std::string vertexShader2 =
			"#version 400\n"
			"in vec3 aPosition;"
			"in vec3 aNormal;"
			"in vec2 aTextureCoord;"
			"out vec3 varNormal;"
			"out vec2 varTextureCoord;"
			"uniform mat4 uModelViewProjectionMatrix;"
			"uniform mat4 uNormalMatrix;"
			"void main() {"
			"varTextureCoord = aTextureCoord;"
			"varNormal = (uNormalMatrix * vec4(aNormal, 1.0)).xyz;"
				"gl_Position = uModelViewProjectionMatrix * vec4(aPosition, 1.0);"
			"}";

		const std::string fragmentShader2 =
			"#version 400\n"
			"layout(location = 0) out vec4 frag_colour;"
			"in vec3 varNormal;"
			"in vec2 varTextureCoord;"
			"uniform sampler2D texture1;"
			"void main() {"
			"float f = dot(vec3(0, 1, 0), varNormal);"
			"vec4 v = (max(f, 0) / 8.0 + 0.825) * texture(texture1, varTextureCoord);"
			"vec4 a = texture(texture1, varTextureCoord);"
			"  frag_colour = v;"
			"}";

		const std::string modelSource =
			"# Blender v2.76 (sub 0) OBJ File : ''\n"
			"# www.blender.org\n"
			"o Cube\n"
			"v 1.000000 -1.000000 -1.000000\n"
			"v 1.000000 -1.000000 1.000000\n"
			"v -1.000000 -1.000000 1.000000\n"
			"v -1.000000 -1.000000 -1.000000\n"
			"v 1.000000 1.000000 -0.999999\n"
			"v 0.999999 1.000000 1.000001\n"
			"v -1.000000 1.000000 1.000000\n"
			"v -1.000000 1.000000 -1.000000\n"
			"vn 0.000000 -1.000000 0.000000\n"
			"vn 0.000000 1.000000 0.000000\n"
			"vn 1.000000 0.000000 0.000000\n"
			"vn -0.000000 0.000000 1.000000\n"
			"vn -1.000000 -0.000000 -0.000000\n"
			"vn 0.000000 0.000000 -1.000000\n"
			"s off\n"
			"f 2//1 3//1 4//1\n"
			"f 8//2 7//2 6//2\n"
			"f 5//3 6//3 2//3\n"
			"f 6//4 7//4 3//4\n"
			"f 3//5 7//5 8//5\n"
			"f 1//6 4//6 8//6\n"
			"f 1//1 2//1 4//1\n"
			"f 5//2 8//2 6//2\n"
			"f 1//3 5//3 2//3\n"
			"f 2//4 6//4 3//4\n"
			"f 4//5 3//5 8//5\n"
			"f 5//6 1//6 8//6\n";

		shader2 = new Shader(vertexShader2, fragmentShader2);
		renderer->setShader(shader2);

		mesh = new Mesh();
		mesh->vertices.push_back(Vertex(Vec(-0.5f, 0.5f, 0.5f), Vec(-0.5f, 0.5f, 0.5f)));
		mesh->vertices.push_back(Vertex(Vec(0.5f, -0.5f, 0.5f), Vec(0.5f, -0.5f, 0.5f)));
		mesh->vertices.push_back(Vertex(Vec(-0.5f, -0.5f, 0.5f), Vec(0.3f, -0.5f, 0.5f)));
		mesh->vertices.push_back(Vertex(Vec(0.5f, -0.5f, 0.5f), Vec(0.5f, -0.5f, 0.5f)));
		mesh->vertices.push_back(Vertex(Vec(-0.5f, 0.5f, 0.5f), Vec(-0.5f, 0.5f, 0.5f)));
		mesh->vertices.push_back(Vertex(Vec(0.5f, 0.5f, 0.5f), Vec(0.5f, 0.5f, 0.5f)));

		//Reporter::report("xyz: " + floatToString(mesh->vertices.begin()->pos.x()) + " " + floatToString(mesh->vertices.begin()->pos.y()) + " " + floatToString(mesh->vertices.begin()->pos.z()));

		renderer->createObject(mesh);

		//Reporter::report("xyz: " + floatToString(mesh->vertices.begin()->pos.x) + " " + floatToString(mesh->vertices.begin()->pos.y) + " " + floatToString(mesh->vertices.begin()->pos.z));


		/*mesh->pos = Vec(0, 0, 2);
		mesh->size = Vec(1, 2, 1);
		mesh->rot = Vec(0, 0, pi / 2.0);*/

		renderer->getCamera()->enableFreeCam(true);
		//glClearColor(100 / 255.0, 149 / 255.0, 237 / 255.0, 1);
		//renderer->setClearColor(1, 1, 0

		//WavefrontModel().loadFromString(modelSource);

		cube = new WavefrontModel("untitled7.obj");
		cube1 = new WavefrontModel("untitled7.obj");
		sphere = new WavefrontModel("untitled8.obj");
		cube->loadTextureFromString("texture2.png");
		cube1->loadTextureFromString("texture.png");

		sphere->loadTextureFromString("texture2.png");
		//cube->loadFromString(modelSource);
		renderer->createObject(cube);
		renderer->createObject(cube1);
		cube->pos = Vec(2, -3, 4);
		cube->size = Vec(3, 1, 3);
		cube->rot = Vec(0, -pi, 0);
		cube1->pos = Vec(2, -2, 4);

		noise = new Texture("noise.png");

		renderer->createObject(sphere);
		sphere->pos = Vec(2, -1, 4);
		sphere->rot = Vec(0, -pi, 0);
		//texture = new Texture("texture.png");
		Reporter::report(glGetError());

		renderer->setFov(70);
	}

	void RenderScene::render(Renderer* renderer)
	{
		renderer->clear();
		//renderer->getCamera()->lookAtNormal(Vec(0, 0, 0));
		//renderer->getCamera()->rotate(camera, 0);
		//glViewport(0, 0, 1200, 720);

		//renderer.setPerspectiveProjection(90, 1200.0 / 720.0, 0.01, 1000);
		//renderer.setCamera(new Camera(Vec(0, 0, 2), Vec(0, 0, 0), Vec(0, 1, 0)));
		
		
		//lm::vec3 debug = glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.01f, 1000.0f) * glm::vec4(0.5, 0.5, 0.5, 1);//renderer.getViewProjectionMatrix().matrix * glm::vec4(0.5, 0.5, 0.5, 1);

		//renderer.setUniformMatrix(renderer.getViewProjectionMatrix(), 3, "MVP");

		/*GLuint textures[2];
		glGenTextures(2, textures);

		int width, height;
		unsigned char* image;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		image = SOIL_load_image("texture.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glUniform1i(glGetUniformLocation(renderer->getShader()->id(), "texture1"), 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		*/
		
		//window resize fixxen!!!
		renderer->setShader(shader2);

		static Depthbuffer depthBuffer(1200, 720);
		depthBuffer.setSize(renderer->getResolution().x, renderer->getResolution().y);
		depthBuffer.bind(renderer);
		renderer->clear();
		cube->render(renderer);
		sphere->render(renderer);
		//sphere->render(renderer);
		depthBuffer.unbind();
		//depthBuffer.render();


		static Normalbuffer normalBuffer(1200, 720);
		normalBuffer.setSize(renderer->getResolution().x, renderer->getResolution().y);
		normalBuffer.bind(renderer);
		renderer->clear();
		cube->render(renderer);
		sphere->render(renderer);
		//sphere->render(renderer);
		normalBuffer.unbind();
		//normalBuffer.render();

		static Framebuffer frameBuffer(1200, 720);
		frameBuffer.setSize(renderer->getResolution().x, renderer->getResolution().y);
		frameBuffer.bind();
		renderer->clear();

		static AmbientOcclusion aa;
		aa.render(renderer, depthBuffer.getTexture(), normalBuffer.getTexture(), *noise);

		frameBuffer.unbind();

		static Framebuffer frameBuffer2(1200, 720);
		frameBuffer2.setSize(renderer->getResolution().x, renderer->getResolution().y);
		frameBuffer2.bind();
		renderer->setShader(shader2);
		renderer->clear();
		cube->render(renderer);
		sphere->render(renderer);
		frameBuffer2.unbind();

		//frameBuffer.render();
		static Blur blur;
		blur.render(renderer, frameBuffer.getTexture(), frameBuffer2.getTexture());

		//buffer.render();
		/*static Framebuffer testbuffer(1200, 720);
		testbuffer.setSize(renderer->getResolution().x, renderer->getResolution().y);
		testbuffer.bind();
		renderer->clear();
		cube->render(renderer);
		testbuffer.unbind();
		cube->render(renderer);
		testbuffer.render();

		cube->render(renderer);*/
		//if (!Keyboard::isKeyDown(KeySpace))
		/*if (!Mouse::isButtonDown(ButtonLeft))*/
			//glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void RenderScene::update(Timestamp timestamp, Camera* camera)
	{
		if (Mouse::isButtonDown(MouseButtons::ButtonLeft))
			Application::getActiveApplication()->getWindow()->setWrap(true);
		if (Mouse::isButtonDown(MouseButtons::ButtonRight))
			Application::getActiveApplication()->getWindow()->setWrap(false);
		Reporter::report(glGetError());
		//camera->update(timestamp, Mouse::getDeltaPos().x, Mouse::getDeltaPos().y);
		//camera -= Mouse::getDeltaPos().y * timestamp.time();
		//Reporter::report(timestamp.getFPS());
		//Reporter::report(floatToString(Mouse::getDeltaPos().x) + " " + floatToString(Mouse::getDeltaPos().y));
	}
}