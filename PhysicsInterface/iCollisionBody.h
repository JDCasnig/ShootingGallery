#ifndef _iCollisionBody_HG_
#define _iCollisionBody_HG_




namespace nPhysics
{
	enum eBodyType
	{
		RIGID_BODY_TYPE = 1,
		SOFT_BODY_TYPE = 2
	};

	// base class/interface for all bodies
	class iCollisionBody
	{
	public:
		virtual ~iCollisionBody() {}

		// eBodyType
		// what kind of body am I? rigid? soft?
		inline eBodyType GetBodyType() const { return mBodyType; }
	private:
		eBodyType mBodyType;
	};
}

#endif