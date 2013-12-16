#ifndef EV_SHADER_H_
#define EV_SHADER_H_

#include "evil.h"

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
        ShaderProgram() { }
		~ShaderProgram();
		void use() const;

		void setVertexShader(std::string s);
		void setFragmentShader(std::string s);

		bool compile();
private:
		GLuint programID = 0;
		Shader vertexShader;
		Shader fragmentShader;
};
}
#endif
