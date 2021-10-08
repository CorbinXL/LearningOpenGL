// Include the "sb7.h" header file

#include "sb7.h"

// Derive my_application from sb7::application

class my_application : public sb7::application

{

public:

	// Our rendering function
	virtual void render(double currentTime)
	{
		const GLfloat color[] = { (float)sin(currentTime) * 0.5f + 0.5f,
		(float)cos(currentTime) * 0.5f + 0.5f,
		0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, color);
		// Use the program object we created earlier for rendering
		glUseProgram(rendering_program);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// Draw one triangle
		glDrawArrays(GL_PATCHES, 0, 3);
	}
	void startup()
	{
		rendering_program = compile_shaders();
		glCreateVertexArrays(1, &vertex_array_object);
		glBindVertexArray(vertex_array_object);
	}

	void shutdown()
	{
		glDeleteVertexArrays(1, &vertex_array_object);
		glDeleteProgram(rendering_program);
		glDeleteVertexArrays(1, &vertex_array_object);
	}


    GLuint compile_shaders(void)
    {
        GLuint vertex_shader;
        GLuint fragment_shader;
        GLuint tessellation_control_shader;
        GLuint tessellation_evaluation_shader;
        GLuint program;
        // Source code for vertex shader
        static const GLchar* vertex_shader_source[] = {
            "#version 410 core \n"
            "void main(void) \n"
            "{ \n"
            "   const vec4 vertices[3] = vec4[3](vec4( 0.25, -0.25, 0.5, 1.0), \n"
            "                                     vec4(-0.25, -0.25, 0.5, 1.0), \n"
            "                                     vec4( 0.25, 0.25, 0.5, 1.0)); \n"
            "   gl_Position = vertices[gl_VertexID];  \n"
            "} \n"
        };
        // Source code for fragment shader
        static const GLchar* fragment_shader_source[] = {
            "#version 410 core \n"
            "out vec4 color; \n"
            " \n"
            "void main(void) \n"
            "{ \n"
            "    color = vec4(0.0, 0.8, 1.0, 1.0);              \n"
            "} \n"
        };

        static const GLchar* control_shader_source[] = {
            "#version 410 core \n"
            "layout (vertices = 3) out; \n"
            "void main(void) \n"
            "{ \n"
            "   if (gl_InvocationID == 0) { \n"
            "       gl_TessLevelInner[0] = 5.0; \n"
            "       gl_TessLevelOuter[0] = 5.0; \n"
            "       gl_TessLevelOuter[1] = 5.0; \n"
            "       gl_TessLevelOuter[2] = 5.0; \n"
            "   } \n"
            "   gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position; \n"
            "} \n"

        };

        static const GLchar* eval_shader_source[] = {
            "#version 410 core \n"
            "layout (triangles, equal_spacing, cw) in; \n"
            "void main(void) \n"
            "{ \n"
            "   gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position + \n"
            "                  gl_TessCoord.y * gl_in[1].gl_Position + \n"
            "                  gl_TessCoord.z * gl_in[2].gl_Position); \n"
            "} \n"
        };

        // Create and compile vertex shader
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
        glCompileShader(vertex_shader);

        tessellation_control_shader = glCreateShader(GL_TESS_CONTROL_SHADER);
        glShaderSource(tessellation_control_shader, 1, control_shader_source, NULL);
        glCompileShader(tessellation_control_shader);

        tessellation_evaluation_shader = glCreateShader(GL_TESS_EVALUATION_SHADER);
        glShaderSource(tessellation_evaluation_shader, 1, eval_shader_source, NULL);
        glCompileShader(tessellation_evaluation_shader);

        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
        glCompileShader(fragment_shader);

        // Create program, attach shaders to it, and link it
        program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, tessellation_control_shader);
        glAttachShader(program, tessellation_evaluation_shader);
        glAttachShader(program, fragment_shader);

        glLinkProgram(program);

        // Delete the shaders as the program has them now
        glDeleteShader(vertex_shader);
        glDeleteShader(tessellation_control_shader);
        glDeleteShader(tessellation_evaluation_shader);
        glDeleteShader(fragment_shader);

        return program;
    }

private:
	GLuint rendering_program;
	GLuint vertex_array_object;

};

// Our one and only instance of DECLARE_MAIN

DECLARE_MAIN(my_application);