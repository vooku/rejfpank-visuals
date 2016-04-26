#include "CDrawable.hpp"

CObject::CObject (const glm::vec3 position)
	: m_position (position),
	  m_triggerTime(0) { }

CObject::~CObject(void) { }
