/**
 * @author	Vadim Petrov
 * @date	2015
 * @brief	Declaration of the common object parent class.
 */

#ifndef _REJFPANK_COBJECT
#define _REJFPANK_COBJECT

#include <string>
#include <iostream>

#include "data.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/quaternion.hpp>
//#include <glm/gtx/quaternion.hpp>
//#include <glm/gtx/rotate_vector.hpp>

class CObject {
public:
	CObject (const glm::vec3 position);
	~CObject (void);

	glm::vec3 position;
	glm::vec3 direction;

protected:

};

#endif // !_REJFPANK_COBJECT