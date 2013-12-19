#ifndef EV_SHADER_H_
#define EV_SHADER_H_

#include "evil.h"
#include "matrix4.h"

#include <string>

namespace evil {

class Shader
{
public:
		Shader() : shaderObject(0) { }
		Shader(std::string s) : source(s), shaderObject(0) { }
		~Shader();

		const GLuint getID() const { return shaderObject; }
		void setSource(std::string s) { source = s; }
		bool compile(GLenum type);
private:
		std::string source;
		GLuint shaderObject;

};

class ShaderProgram
{
public:
		ShaderProgram() : programID(0) { }
		~ShaderProgram();
		void use() const;

		void setUniformMatrix(const std::string& s, const Matrix4& matrix) const;
		void setVertexShader(std::string s);
		void setFragmentShader(std::string s);
		const GLint getAttribLocation(const std::string& name) const;
		const GLint getUniformLocation(const std::string &name) const;
		bool compile();
		bool checkProgram();
private:
		GLuint programID;
		Shader vertexShader;
		Shader fragmentShader;
};
}
#endif
