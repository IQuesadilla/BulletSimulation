#pragma once

#define GL_GLEXT_PROTOTYPES

#include <string>
#include <GL/gl.h>
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <fstream>
#include <sstream>
#include <memory.h>
#include <iostream>
#include <tiny_obj_loader.h>
#include <json/json.h>

class _shader
{
public:
    _shader();
    ~_shader();

    bool load(std::string vertex, std::string fragment);
    bool generate(tinyobj::material_t material, std::vector<std::string> layout);
    void use();
    //GLint ID() {return programID;};

	void setJsonValues(Json::Value inshadervalues);
	void updateJsonValues();

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec2(const std::string &name, const glm::vec2 &value) const;
    void setVec2(const std::string &name, float x, float y) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    void setVec4(const std::string &name, const glm::vec4 &value) const;
    void setVec4(const std::string &name, float x, float y, float z, float w);
    void setMat2(const std::string &name, const glm::mat2 &mat) const;
    void setMat3(const std::string &name, const glm::mat3 &mat) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
    std::string readFile(std::string path);
    bool compile(std::string vertexsource, std::string fragmentsource);
    bool checkCompileErrors(GLuint shader, std::string type);

	Json::Value shadervalues;

	uint programID;
    bool run;
};