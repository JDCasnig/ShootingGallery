#ifndef _cRigidBodyDef_HG_
#define _cRigidBodyDef_HG_

#include "GLMIncludes.h"
namespace nPhysics
{
	class cRigidBodyDef
	{
	public:
		cRigidBodyDef();
		~cRigidBodyDef();

		glm::vec3 position;
		glm::vec3 velocity;
		float mass;
		glm::vec3 force;
		glm::quat qOrientation;
		bool staticBody;
	private:
		int rigidbody = 1;
	};
}
#endif