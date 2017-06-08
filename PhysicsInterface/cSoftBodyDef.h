#ifndef _cSoftBodyDef_HG_
#define _cSoftBodyDef_HG_

#include <vector>
#include "GLMIncludes.h"
#include "cRigidBodyDef.h"
namespace nPhysics
{
	struct sNodeConstraint
	{

		sNodeConstraint(nPhysics::cRigidBodyDef* nodeA, nPhysics::cRigidBodyDef* nodeB, float tensorStrength, float engagedDistance, float maxAllowedLength);
		~sNodeConstraint();
		nPhysics::cRigidBodyDef* nodeA;
		nPhysics::cRigidBodyDef* nodeB;

		float tensorStrength;
		//float relaxedDistance;
		float engagedDistance;
		float maxAllowedLength;
	};


	class cSoftBodyDef
	{
	public:
		cSoftBodyDef(std::vector<cRigidBodyDef*> nodes, std::vector<sNodeConstraint> nodeConstraints);
		~cSoftBodyDef();

		std::vector<cRigidBodyDef*> nodes;
		std::vector<sNodeConstraint> nodeConstraints;


	};
}
#endif