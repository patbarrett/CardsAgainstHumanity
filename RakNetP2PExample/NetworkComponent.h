#ifndef NETWORKCOMPONENT
#define NETWORKCOMPONENT

#include "../GameExample/BaseComponent.h"

class CNetworkComponent : public CBaseComponent
{
public:
	CNetworkComponent(void);
	virtual ~CNetworkComponent(void);

	virtual void Update(unsigned int deltaTime) {}
};

#endif

