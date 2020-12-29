#include "shader.h"

_shader::_shader()
{
    programID = 0;
    run = false;
}

_shader::~_shader()
{

}

bool _shader::load(std::string vertex, std::string fragment)
{
    std::string vertexsource = readFile(vertex);
    std::string fragmentsource = readFile(fragment);

    return compile(vertexsource, fragmentsource);
}

bool _shader::generate(tinyobj::material_t material, std::vector<std::string> layout)
{
    std::stringstream vertexsource, fragmentsource;

    vertexsource << "#version 330 core" << std::endl;
    fragmentsource << "#version 330 core" << std::endl;

    for (int x = 0; x < layout.size(); x++)
    {
        vertexsource << "layout (location = " << x << ") in " << layout.at(x) << " LAY" << x << ';' << std::endl;
    }
    vertexsource << std::endl;

    for (int x = 0; x < layout.size(); x++)
    {
        vertexsource << "out " << layout.at(x) << " VTF" << x << ';' << std::endl;
        fragmentsource << "in " << layout.at(x) << " VTF" << x << ';' << std::endl;
    }

    vertexsource << std::endl << "uniform mat4 projection;" << std::endl << "uniform mat4 view;" << std::endl << "uniform mat4 model;" << std::endl;

    return true;
}

void _shader::use()
{
    if (run) glUseProgram (programID);
}

void _shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
}

void _shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void _shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void _shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
    glUniform2fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

void _shader::setVec2(const std::string &name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(programID, name.c_str()), x, y);
}

void _shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

void _shader::setVec3(const std::string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(programID, name.c_str()), x, y, z);
}
void _shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
    glUniform4fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}
void _shader::setVec4(const std::string &name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(programID, name.c_str()), x, y, z, w);
}

void _shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void _shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void _shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

std::string _shader::readFile(std::string path)
{
    std::ifstream file;
    file.open(path);
    std::stringstream data;
    data << file.rdbuf();
    file.close();
    return data.str();
}

bool _shader::compile(std::string vertexsource, std::string fragmentsource)
{
    GLuint vertexshader, fragmentshader;
    bool error = false;

    const char *vertexdata = vertexsource.c_str();
    const char *fragmentdata = fragmentsource.c_str();

    vertexshader = glCreateShader (GL_VERTEX_SHADER);
	glShaderSource (vertexshader, 1, &vertexdata, NULL);
	glCompileShader (vertexshader);
    error = checkCompileErrors(vertexshader,"VERTEX");

	fragmentshader = glCreateShader (GL_FRAGMENT_SHADER);
	glShaderSource (fragmentshader, 1, &fragmentdata, NULL);
	glCompileShader (fragmentshader);
    error = checkCompileErrors(fragmentshader,"FRAGMENT");

	programID = glCreateProgram ();
	glAttachShader (programID, vertexshader);
	glAttachShader (programID, fragmentshader);
	glLinkProgram (programID);
    error = checkCompileErrors(programID,"PROGRAM");

	glDeleteShader (vertexshader);
	glDeleteShader (fragmentshader);

    if (error) std::cout << vertexsource << std::endl << fragmentsource << std::endl;
    run = true;
    return error;
}

bool _shader::checkCompileErrors(GLuint shader, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if(type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        //printf("%d\n",success);
        if(!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    return !success;
}
