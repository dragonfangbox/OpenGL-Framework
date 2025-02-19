#version 400 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;

	uniform mat4 transform;
	uniform mat4 scale;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	 out vec3 ourColor;

    void main() {
    
		gl_Position = projection * view * (model * (transform * scale)) * vec4(aPos, 1.0f);
		ourColor = aColor;
    }; 
