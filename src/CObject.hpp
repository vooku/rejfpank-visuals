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

class CObject {
public:
	CObject (const glm::vec3 position);
	virtual ~CObject(void);

	glm::vec3 m_position;
	glm::vec3 m_direction;

	double m_triggerTime;
protected:
	
};

#endif // !_REJFPANK_COBJECT