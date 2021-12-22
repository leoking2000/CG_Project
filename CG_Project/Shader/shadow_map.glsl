#shader vertex
#version 330
layout(location = 0) in vec3 apos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * model * vec4(apos, 1.0);
}

#shader fragment
#version 330

void main()
{
    
}
