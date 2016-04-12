/**
 * @file	pgr.hpp
 * @autho	Tomas Barak, Petr Felkel
 * @brief	Helper functions for shader loading, compiling, deleting and texture loading
 * @date	2011-2012
 */

#ifndef _PGR
#define _PGR

#include <vector>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <IL/il.h>

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

/** Simple texture loading helper
*
* The function creates 1 2D texture (calling glGenTextures), loads its image using pgr::loadTexImage2D,
* sets filtering (calling glTexParameteri) and also creates mipmaps if required (using glGenerateMipmap).
*
* The texture will be created with linear filtering and mip-mapping enabled by default.
* You can disable mip-mapping and mipmap generation setting the mipmap parameter to false.
*
* \param fileName the texture file name
* \param mipmap Enable and generate mipmaps for this texture
* \return texture usable in OpenGL or 0 on failure
*/
GLuint createTexture(const std::string &fileName, bool mipmap = true);

/** Load texture from file and uplad texels to opengl using glTexImage2D
*
* The functions will load texels from fileName and call glTexImage2D,
* so the data will replace image of the currently texture, currently bound to target.
*
* \note The DevIL library is used internally. Make sure ilInit() is called before this function (for example in pgr::initialize()).
* \note The function has to be called after successful creation of the OpenGL context.
* \warning Make sure ilInit() was called prior to this function (for example in pgr::initialize())
* \param fileName file to open and load as a texture
* \param target which texture target will be used as a parameter of glTexImage2D (GL_TEXTURE_2D for example)
* \return true if texture was successfully loaded from fileName
*/
bool loadTexImage2D(const std::string & fileName, GLenum target);

} // end namespace pgr

#endif // !_PGR
