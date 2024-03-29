#include "TexturedQuad.h"
#include "../Render.h"

TexturedQuad::TexturedQuad(unsigned int& _texture, glm::vec3 _position, int _width, int _height)
{
	position = _position;
	width = _width;
	height = _height;

	// Define Vertices
	vertices.push_back(BufferData(
		glm::vec3(1.0f, 1.0f, 0.0f),   // pos
		glm::vec2(1.0f, 1.0f)		    // uv
	));  // top right
	vertices.push_back(BufferData(
		glm::vec3(1.0f, -1.0f, 0.0f),    // pos
		glm::vec2(1.0f, 0.0f)		    // uv
	)); // bottom right
	vertices.push_back(BufferData(
		glm::vec3(-1.0f, -1.0f, 0.0f),  // pos
		glm::vec2(0.0f, 0.0f)		    // uv
	)); // bottom left
	vertices.push_back(BufferData(
		glm::vec3(-1.0f, 1.0f, 0.0f),	// pos
		glm::vec2(0.0f, 1.0f)			// uv
	)); // top left

	// Define Indices
	indices = {
		0, 1, 3,
		1, 2, 3
	};

	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("shaders/SimpleTexture.vertexshader", "shaders/SimpleTexture.fragmentshader");

	BindVAO();
	BindVBO();
	BindEBO();

	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(programID, "MVP");

	glm::mat4 Projection = Render::getInstance()->Projection;
	glm::mat4 View = Render::getInstance()->View;

	model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, glm::vec3(_width, _height, 0.0f));
	MVP = Projection * View * model;

	texture = _texture;
}

TexturedQuad::~TexturedQuad()
{
}

void TexturedQuad::Draw()
{
	glUseProgram(programID);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		sizeof(BufferData), // stride
		(void*)0            // array buffer offset
	);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(
		1,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		sizeof(BufferData), // stride
		(void*)(offsetof(BufferData, v3Color))            // array buffer offset
	);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(
		2,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		sizeof(BufferData), // stride
		(void*)(offsetof(BufferData, v2UV))            // array buffer offset
	);

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniform1i(glGetUniformLocation(programID, "circleMask"), textureMask);


	glBindTexture(GL_TEXTURE_2D, texture);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);

	glDisableVertexAttribArray(0);
}

void TexturedQuad::Update()
{
	glm::mat4 Projection = Render::getInstance()->Projection;
	glm::mat4 View = Render::getInstance()->View;

	model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, glm::vec3(width, height, 0.0f));
	MVP = Projection * View * model;
}

void TexturedQuad::UpdateTexture(unsigned int& _texture)
{
	texture = _texture;
}
