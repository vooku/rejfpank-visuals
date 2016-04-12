#include "CDrawable.hpp"

using namespace std;

CObject::CObject (const glm::vec3 position)
	: m_position (position),
	  m_triggerTime(0) { }