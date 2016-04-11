#include "CDrawable.hpp"

using namespace std;

CObject::CObject (const glm::vec3 position)
	: position (position),
	  triggerTime(0) { }

CObject::~CObject (void) { }