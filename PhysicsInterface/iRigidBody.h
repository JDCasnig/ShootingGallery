#ifndef _iRigidBody_HG_
#define _iRigidBody_HG_




#include "iCollisionBody.h"
#include "cRigidBodyDef.h"
#include "iShape.h"


namespace nPhysics
{
	class iRigidBody : public iCollisionBody
	{
	public:
		virtual ~iRigidBody() {}

		virtual cRigidBodyDef* GetRigidBodyDef() = 0;
		virtual iShape* GetShape() = 0;

	};
}

#endif // !_iRigidBody_HG_