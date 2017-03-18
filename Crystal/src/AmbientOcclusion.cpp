#include "AmbientOcclusion.h"

namespace Crystal
{
	char* AmbientOcclusion::ssaoSource =
		"#version 330 core\n"
		"out vec4 FragColor;"
		"in vec2 varTextureCoord;"

		"uniform sampler2D gPositionDepth;"
		"uniform sampler2D gNormal;"
		"uniform sampler2D texNoise;"

		"uniform vec3 samples[64];"

		"uniform mat4 projection;"
		"uniform mat4 aInverseProjectionMatrix;"
		"int kernelSize = 64;"
		"float radius = 0.3;"

		"const vec2 noiseScale = vec2(1200.0f / 4.0f, 720.0f / 4.0f);"


		"void main()"
		"{"
		"vec4 pos = vec4(varTextureCoord.x, varTextureCoord.y, 1.0, 1.0);"
		"pos.xyz = pos.xyz * 2.0 - 1.0;"
		"pos = aInverseProjectionMatrix * pos;"
		"vec3 fragPos = pos.xyz /* pos.w*/ * (texture(gPositionDepth, varTextureCoord).x + texture(gPositionDepth, varTextureCoord).y / 255.0 / 255.0 /*+ texture(gPositionDepth, varTextureCoord).z / 255.0 / 255.0 / 255.0*/);"
		"fragPos.z = texture(gPositionDepth, varTextureCoord).x +  texture(gPositionDepth, varTextureCoord).y / 255.0 / 255.0 /*+ texture(gPositionDepth, varTextureCoord).z / 255.0 / 255.0 / 255.0*/;"
		"vec3 normal = normalize(texture(gNormal, varTextureCoord).xyz * 2.0 - 1.0);"
		"vec3 randomVec = texture(texNoise, varTextureCoord * noiseScale).xyz;"
		"vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));"
		"vec3 bitangent = cross(normal, tangent);"
		"mat3 TBN = mat3(tangent, bitangent, normal);"
		"float occlusion = 0.0;"
		"for (int i = 0; i < kernelSize; ++i)"
		"{"
		"vec3 sample = TBN * samples[i];"
		"sample = fragPos + sample * radius;"

		"vec4 offset = vec4(sample, 1.0);"
		"offset = projection * offset;"
		"offset.xyz /= offset.w;"
		"offset.xyz = offset.xyz * 0.5 + 0.5;"

		"float sampleDepth = texture(gPositionDepth, (1 - offset.xy)).x;"

		"float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));"
		"occlusion += (sampleDepth + 0.00/*4*/ <= sample.z ? 1.0 : 0.0) * rangeCheck;"

		"}"
		"occlusion = 1.0 - (occlusion / float(kernelSize));"
		"occlusion = pow(occlusion, 1.0);"
		//"FragColor = vec4(-pos.w, -pos.w, -pos.w, 1.0);"
		"FragColor = vec4(occlusion, occlusion, occlusion, 1.0);"
		//"FragColor = vec4(fragPos.x, fragPos.y, fragPos.z * 50.0, 1.0);"
		//"FragColor = vec4(fragPos.z / 50.0, fragPos.z / 50.0, fragPos.z / 50.0, 1.0);"
		//"FragColor = vec4(texture(gPositionDepth, varTextureCoord).z , texture(gPositionDepth, varTextureCoord).z , texture(gPositionDepth, varTextureCoord).z , 1.0);"
		//"FragColor = vec4((normal.x + 1) / 2.0, (normal.y + 1) / 2.0, (normal.z + 1) / 2.0, 1.0);"
		"}";
		/*"#version 450\n"
		"out vec4 frag_colour;"
		"in vec2 varTextureCoord;"
		"uniform mat4 aProjectionMatrix;"
		"uniform mat4 aInverseProjectionMatrix;"
		"uniform sampler2D texture0;"
		"uniform sampler2D texture1;"
		"uniform sampler2D texture2;"

		"float GetDistance(in vec2 texCoord)"
		"{"
		//"return texture(texture0, texCoord).r;"
		"const vec4 bitSh = vec4(1.0 / 16777216.0, 1.0 / 65535.0, 1.0 / 256.0, 1.0);"
		"return dot(texture(texture0, texCoord), bitSh) * 10;"
		"}"

		"vec3 GetViewPos(in vec2 texCoord)"
		"{"
		"	float depth = (GetDistance(texCoord)) * 10.0;"
		"	vec4 pos = vec4(texCoord.x, texCoord.y, depth, 1.0);"
		"	pos.xyz = pos.xyz * 2.0 - 1.0;"
		"	pos = aInverseProjectionMatrix * pos;"
		"	return pos.xyz / pos.w;"
		"}"

		"void main()"
		"{"
		// Modifiable settings
		"	const float ssaoFocus = 1.5;"
		"	const float ssaoPower = 8.0;"
		"	const int   ssaoLoops = 32;"

		"	vec2 texCoord = varTextureCoord;"

		// View space normal
		"vec3 normal = normalize(texture(texture1, texCoord).xyz * 2.0 - 1.0);"

		// View space position of the pixel
		"vec3 pos = GetViewPos(texCoord);"
		"pos.z = pos.z;"

		// Random value sampled from the texture in repeated screen coordinates (32 x 32)
		"vec2 modifier = texture(texture2, texCoord / 32.0).xy + (pos.xy + pos.z);"

		"float dist, visibility = 0.0;"
		"vec4 random, screenPos, viewPos = vec4(1.0);"

		"for (int i = 0; i < ssaoLoops; i++)"
		"{"
		// Retrieve a new random vector from the texture
		"	random = texture(texture2, modifier);"

		// Not much point in normalizing -- no visual difference
		"	random.xyz = random.xyz * 2.0 - 1.0;"

		// Randomize the modifier for the next loop
		"	modifier += random.xy;"

		// Flip the random vector if it's below the plane
		"if (dot(random.xyz, normal) < 0.0) random.xyz = -random.xyz;"

		// Randomly offset view-space position
		"viewPos.xyz = random.xyz * (ssaoFocus * random.w) + pos;"

		// Calculate the randomly offset position's screen space coordinates -- second most expensive operation
		"screenPos = aProjectionMatrix * viewPos;"
		"dist = GetDistance(screenPos.xy / screenPos.w * 0.5 + 0.5);"

		// Visibility is linearly scaled, depending on how far the distance is from the focus range
		"visibility += min(abs((viewPos.z + dist) / ssaoFocus + 1.0), 1.0);"
		"}"

		// Final occlusion factor
		//"frag_colour = vec4(pow(visibility / float(ssaoLoops), ssaoPower));"
		"frag_colour = vec4(pos.z, pos.z, pos.z, 1);"
		"} ";*/
		/*"#version 400 core\n"
		"out vec4 frag_colour;"
		"in vec2 varTextureCoord;"
		"uniform sampler2D texture0;    // Depth in linear format"
		"uniform sampler2D texture1;    // Normal"
		"uniform sampler2D texture2;    // Random"
		"uniform mat4 aProjectionMatrix;   // Current projection matrix"
		"uniform mat4 aInverseProjectionMatrix;  // Inverse projection matrix"

		"float GetDistance(in vec2 texCoord)"
		"{"
		"	return texture2D(texture0, texCoord).r * 1;"
		"	//const vec4 bitSh = vec4(1.0 / 16777216.0, 1.0 / 65535.0, 1.0 / 256.0, 1.0);"
		"	//return dot(texture2D(texture0, texCoord), bitSh);"
		"}"

		"vec3 GetViewPos(in vec2 texCoord)"
		"{"
		"	float depth = GetDistance(texCoord);"
		"	vec4 pos = vec4(texCoord.x, texCoord.y, depth, 1.0);"
		"	pos.xyz = pos.xyz * 2.0 - 1.0;"
		"	pos = aInverseProjectionMatrix * pos;"
		"	return pos.xyz / pos.w;"
		"}"

		"void main()"
		"{"
		"	// Modifiable settings"
		"	const float ssaoFocus = 1.5;"
		"	const float ssaoPower = 8.0;"
		"	const int   ssaoLoops = 32;"
		// Texture coordinate"
		"	vec2 texCoord = varTextureCoord;"

		"	// View space normal"
		"	vec3 normal = normalize(texture2D(texture1, texCoord).xyz * 2.0 - 1.0);"

		"	// View space position of the pixel"
		"	vec3 pos = GetViewPos(texCoord);"

		"	// Random value sampled from the texture in repeated screen coordinates (32 x 32)"
		"	vec2 modifier = texture2D(texture2, texCoord / 32.0).xy + (pos.xy + pos.z);"

		"	float dist, visibility = 0.0;"
		"	vec4 random, screenPos, viewPos = vec4(1.0);"

		"	for (int i = 0; i < ssaoLoops; i++)"
		"	{"
		"		// Retrieve a new random vector from the texture"
		"		random = texture2D(texture2, modifier);"

		"		// Not much point in normalizing -- no visual difference"
		"		random.xyz = random.xyz * 2.0 - 1.0;"

		"		// Randomize the modifier for the next loop"
		"		modifier += random.xy;"

		"		// Flip the random vector if it's below the plane"
		"		if (dot(random.xyz, normal) < 0.0) random.xyz = -random.xyz;"

		"		// Randomly offset view-space position"
		"		viewPos.xyz = random.xyz * (ssaoFocus * random.w) + pos;"

		"		// Calculate the randomly offset position's screen space coordinates -- second most expensive operation"
		"		screenPos = aProjectionMatrix * viewPos;"

		"		// Get the depth at the screen space coordinates -- this is the most expensive operation"
		"		dist = GetDistance(screenPos.xy / screenPos.w * 0.5 + 0.5);"

		"		// Visibility is linearly scaled, depending on how far the distance is from the focus range"
		"		visibility += min(abs((viewPos.z + dist) / ssaoFocus + 1.0), 1.0);"
		"	}"

		"	// Final occlusion factor"
		"	frag_colour = vec4(pow(visibility / float(ssaoLoops), ssaoPower));"
		"}";*/

	char* testSource = 
		"#version 400\n"
		"out vec3 color;"
		"uniform sampler2D texture0;"
		"in vec2 varTextureCoord;"
		"void main() {"
		"color = texture(texture0, varTextureCoord).xyz;"
		"}";

	AmbientOcclusion::AmbientOcclusion() : ScreenEffect(ssaoSource)
	{
		std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // random floats between 0.0 - 1.0
		std::default_random_engine generator;
		for (GLuint i = 0; i < 64; ++i)
		{
			glm::vec3 sample(
				randomFloats(generator) * 2.0 - 1.0,
				randomFloats(generator) * 2.0 - 1.0,
				randomFloats(generator)
				);
			sample = glm::normalize(sample);
			sample *= randomFloats(generator);
			GLfloat scale = GLfloat(i) / 64.0;

			scale = 0.1 + scale * scale * (1.0 - 0.1);
			sample *= scale;
			ssaoKernel.push_back(sample);
		}

		std::uniform_real_distribution<GLfloat> randomFloats2(0.0, 1.0);

		std::vector<glm::vec3> ssaoNoise;
		for (GLuint i = 0; i < 16; i++)
		{
			glm::vec3 noise(
				randomFloats2(generator) * 1 + 0.0,
				randomFloats2(generator) * 1 + 0.0,
				0.0f);
			ssaoNoise.push_back(noise);
		}

		glGenTextures(1, &noiseTexture);
		glBindTexture(GL_TEXTURE_2D, noiseTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		for (auto oneKernel : ssaoKernel)
		{
			kernel.push_back(oneKernel.x);
			kernel.push_back(oneKernel.y);
			kernel.push_back(oneKernel.z);
		}
	}

	void AmbientOcclusion::render(Renderer* renderer, Texture depth, Texture normal, Texture noise)
	{

		Reporter::report(glGetError());
		//shader = Shader()
		shader.bind();
		depth.bind(0);
		normal.bind(1);
		//noise.bind(2);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, noiseTexture);

		glBindVertexArray(vao);

		Reporter::report(glGetError());

		GLint location = glGetUniformLocation(shader.id(), "projection");
		glUniformMatrix4fv(location, 1, GL_FALSE, (renderer->getProjectionMatrix()).get());

		location = glGetUniformLocation(shader.id(), "samples");
		glUniform3fv(location, kernel.size() * 3, &kernel[0]);

		location = glGetUniformLocation(shader.id(), "aInverseProjectionMatrix");
		glUniformMatrix4fv(location, 1, GL_FALSE, (renderer->getInverseProjectionMatrix()).get());

		GLint textureLocation = glGetUniformLocation(shader.id(), "gPositionDepth");
		glUniform1i(textureLocation, 0);

		textureLocation = glGetUniformLocation(shader.id(), "gNormal");
		glUniform1i(textureLocation, 1);

		textureLocation = glGetUniformLocation(shader.id(), "texNoise");
		glUniform1i(textureLocation, 2);


		glDrawArrays(GL_TRIANGLES, 0, 18 * sizeof(float));
	}
}