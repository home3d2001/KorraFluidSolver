#include "shaderProgram.h"
#include "loadShaders.h"

ShaderProgram::ShaderProgram(
        const char* vertFilePath,
        const char* fragFilePath
        )
{
    m_programID = LoadShaders(vertFilePath, fragFilePath);
}

void ShaderProgram::Render(
    const Geometry* geo
    )
{
    glUseProgram(m_programID);

    // GLuint VertexArrayID = 0;
    // glGenVertexArrays(1, &VertexArrayID);
    // glBindVertexArray(VertexArrayID);

    // // Draw a triangle
    // static const GLfloat g_vertex_buffer_data[] = {
    //     -1.0f, -1.0f, 0.0f,
    //     1.0f, -1.0f, 0.0f,
    //     0.0f, 1.0f, 0.0f
    // };

    // // Identify vertex buffer
    // GLuint vertexbuffer;


    // // Generate one and put the result in vertexbuffer
    // glGenBuffers(1, &vertexbuffer);

    // glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // // Give vertices to openGL
    // glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // // First attribute buffer
    // glEnableVertexAttribArray(0);
    // glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // glVertexAttribPointer(
    //     0, // Attribute 0, no reason just need to match the layout in the shader
    //     3, // size
    //     GL_FLOAT, // type
    //     GL_FALSE, // normalized?
    //     0, // stride
    //     (void*)0 // array buffer offset
    // );

    // glPointSize(5.0f);

    // // Draw the triangle!
    // glDrawArrays(GL_TRIANGLES, 0, 3); // vertex 3, count 3 -> 1 triangle
    // glDisableVertexAttribArray(0);
    geo->Bind();
    // glDrawElements(GL_POINTS, geo.ElementCount(), GL_UNSIGNED_SHORT, NULL);
    glDrawArrays(GL_TRIANGLES, 0, 3); // vertex 3, count 3 -> 1 triangle
}
