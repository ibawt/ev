#include "evil.h"
using namespace std;

namespace evil {

Shader::~Shader()
{
		if( shaderObject ) {
				glDeleteShader(shaderObject);
				shaderObject = 0;
		}
}

bool Shader::compile(GLenum type )
{

		shaderObject = glCreateShader(type);
		if( !shaderObject )
				return false;
				const char *data = source.c_str();
		glShaderSource(shaderObject, 1, (const GLchar **)&data,0);

		glCompileShader(shaderObject);

		int status;
		glGetShaderiv(shaderObject,GL_COMPILE_STATUS,&status);
		if(status == GL_FALSE)
		{
				int loglength;
				glGetShaderiv(shaderObject,GL_INFO_LOG_LENGTH,&loglength);

				string data;
				data.resize(loglength);

				glGetShaderInfoLog(shaderObject,loglength,&loglength,&data[0]);

				error("can't compile program: %s\n", data.c_str());
				return false;
		}
		return true;
}
ShaderProgram::~ShaderProgram()
{
		if( programID ) {
				glDeleteProgram(programID);
				programID = 0;
		}
}

void ShaderProgram::use() const
{
		if( programID ) {
				glUseProgram(programID);
		}
}

void ShaderProgram::setVertexShader(string s)
{
		vertexShader.setSource(s);
}

void ShaderProgram::setFragmentShader(string s )
{
		fragmentShader.setSource(s);
}

bool ShaderProgram::compile()
{
		if( !vertexShader.compile(GL_VERTEX_SHADER) )
				return false;

		if( !fragmentShader.compile(GL_FRAGMENT_SHADER) )
				return false;

		programID = glCreateProgram();
		if( !programID ) {
				return false;
		}

		glAttachShader(programID, vertexShader.getID());
		checkProgram();
		glAttachShader(programID, fragmentShader.getID());
		checkProgram();
		glLinkProgram(programID);
		checkProgram();
		return true;
}

bool ShaderProgram::checkProgram()
{
		int loglength;
		string info;

		glGetProgramiv( programID,GL_INFO_LOG_LENGTH, &loglength);

		if( loglength > 0 ) {
				info.resize(loglength);
				glGetProgramInfoLog(programID, loglength, &loglength, &info[0]);
				log("Program IV: %s", info.c_str() );
		}
		return true;
}

const GLint ShaderProgram::getUniformLocation(const string& name) const
{
		return glGetUniformLocation(programID, name.c_str());
}

const GLint ShaderProgram::getAttribLocation(const string& name) const
{
		return glGetAttribLocation(programID, name.c_str());
}

void ShaderProgram::setUniformMatrix(const string& name, const Matrix4& matrix) const
{
		glUniformMatrix4fv(getUniformLocation( name), 1, false, matrix.m);
}


}
