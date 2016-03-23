/**
 * \file Shader.h
 * \author Tomas Barak, Petr Felkel
 * \brief Helper functions for shader loading, compiling and deleting
 * \date 2011-2012
 */

#ifndef _PGR_SHADER
#define _PGR_SHADER

#include <vector>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace pgr {

/** prints out pending opengl error
 *
 * If there is nothing to print (no opengl error has occurred), the function prints nothing.
 * You can use where and line arguments to supply addtional information that will be printed out.
 *
 * \see CHECK_GL_ERROR
 */
void checkGLError(const char * where = 0, int line = 0);

/** helper macro involving the checkGLError function
 *
 * The macro uses __FUNCTION__ and __LINE__ as arguments to the checkGLError function.
 * Use it anywhere you like to catch opengl errors.
 */
#define CHECK_GL_ERROR() do { pgr::checkGLError(__FUNCTION__, __LINE__); } while(0)

/** helper function for shader compiling
 *
 * The function internaly uses glCreateShader, glShaderSource and glCompileShader sequence.
 * glGetShaderiv is used to detect compile errors, the function prints GLSL errors and warnings to stderr.
 *
 * \param eShaderType we support GL_VERTEX_SHADER, GL_FRAGMENT_SHADER and GL_GEOMETRY_SHADER values
 * \param strShaderText GLSL code to be used as a glShaderSource argument
 * \return opengl shader id or 0 on failure
 */
GLuint createShaderFromSource(GLenum eShaderType, const std::string &strShaderText);

/** helper function for shader compiling and loading
 *
 * This funcstion loads shader source from file named filename and calls createShaderFromSource().
 *
 * \param eShaderType we support GL_VERTEX_SHADER, GL_FRAGMENT_SHADER and GL_GEOMETRY_SHADER values
 * \param filename file to use as a shader source
 * \return opengl shader id or 0 on failure
 */
GLuint createShaderFromFile(GLenum eShaderType, const std::string &filename);

/** helper function for shader linking
 *
 * The function internaly uses glCreateProgram, glAttachShader to attach all supplied shaders and finally glLinkProgram.
 * Any GLSL errors and warnings are printed to stderr.
 *
 * \param shaderList array of compiled shaders to attach, you can use createShaderFromSource() for example to fill the array
 * \return opengl shader program id or 0 on failure
 */
GLuint createProgram(const std::vector<GLuint> &shaderList);

/** helper function for shader linking - overloaded
 *
 * This is the overloaded version of createProgram().
 *
 * \see createProgram
 * \param shaders zero ended array of shader ids - for example {vsid, fsid, 0}
 * \return opengl shader program id or 0 on failure
 */
GLuint createProgram(const GLuint * shaders);

/** deletes the shader program and all attached shaders
 *
 * You should delete all shader programs before your application terminates.
 */
void deleteProgramAndShaders( GLuint theProgram );

} // end namespace pgr

#endif // !_PGR_SHADER
