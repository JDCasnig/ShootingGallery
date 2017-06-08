#include "cJSoftBody.h"


cJSoftBody::cJSoftBody(nPhysics::cSoftBodyDef* sbDef)
{
	bodyDef = sbDef;
}

cJSoftBody::~cJSoftBody() {}

nPhysics::cSoftBodyDef* cJSoftBody::GetSoftBodyDef()
{
	return bodyDef;
}

nPhysics::iShape* cJSoftBody::GetShape()
{
	return NULL;
}


