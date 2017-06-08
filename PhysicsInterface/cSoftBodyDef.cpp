#include "cSoftBodyDef.h"

	nPhysics::cSoftBodyDef::cSoftBodyDef(std::vector<cRigidBodyDef*> nodes, std::vector<nPhysics::sNodeConstraint> nodeConstraints)
	{
		this->nodes = nodes;
		this->nodeConstraints = nodeConstraints;

	}
	nPhysics::cSoftBodyDef::~cSoftBodyDef()
	{

	}

	nPhysics::sNodeConstraint::sNodeConstraint(nPhysics::cRigidBodyDef * nodeA, nPhysics::cRigidBodyDef * nodeB, float tensorStrength, float engagedDistance, float maxAllowedLength)
	{
		this->nodeA = nodeA;
		this->nodeB = nodeB;
		this->tensorStrength = tensorStrength;
		this->engagedDistance = engagedDistance;
		this->maxAllowedLength = maxAllowedLength;
	}

	nPhysics::sNodeConstraint::~sNodeConstraint()
	{
	}
