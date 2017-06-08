#ifndef _iSoftBody_HG_
#define _iSoftBody_HG_




#include "iCollisionBody.h"
#include "cRigidBodyDef.h"
#include "cSoftBodyDef.h"
#include "iShape.h"


namespace nPhysics
{
	class iSoftBody : public iCollisionBody
	{
	public:
		virtual ~iSoftBody() {}

		virtual cSoftBodyDef* GetSoftBodyDef() = 0;
		virtual iShape* GetShape() = 0;

	};
}

#endif // !_iSoftBody_HG_