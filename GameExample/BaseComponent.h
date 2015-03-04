#ifndef BASECOMPONENT
#define BASECOMPONENT

class CGameObject;

class CBaseComponent
{
public:
	CBaseComponent(void);
	~CBaseComponent(void);

	void SetOwner(CGameObject *owner) {m_Owner = owner;}
	void Update(unsigned int deltaTime) {}

	unsigned int GetID() const {return m_ID;}

private:
	unsigned int m_ID;
	CGameObject *m_Owner;
};

#endif 

