#ifndef _cJSoftBody_HG_
#define _cJSoftBody_HG_



#include <iSoftBody.h>
#include <cSoftBodyDef.h>


	class cJSoftBody : public nPhysics::iSoftBody
	{
	public:
		cJSoftBody(nPhysics::cSoftBodyDef* sbDef);
		~cJSoftBody();

		nPhysics::cSoftBodyDef* GetSoftBodyDef();
		nPhysics::iShape* GetShape();


	private:
		nPhysics::cSoftBodyDef* bodyDef;
	};


#endif // !_iSoftBody_HG_