
const char* vertexShaderCode =
"#version 430\r\n"
""
"in layout(location=0) vec2 position;"
"in layout(location=1) vec3 vertexColor;"
""
"out vec3 tempColor;"
""
"void main()"
"{"
"	gl_Position = vec4(position, 0.0, 1.0);"
"	tempColor = vertexColor;"
"}"
"";

const char* fragmentShaderCode =
"#version 430\r\n"
""
"out vec4 color;"
"in vec3 tempColor;"
""
"void main()"
"{"
"	color = vec4(tempColor, 1.0);"
"}";
