#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>

class CBaseComponent;

class GameObject 
{
public:
	GameObject();
	~GameObject() {}

	void GetPosition(float &outX,float &outY, float &outZ ) const;
	void SetPosition(float x, float y, float z);
	void PrintPosition() const;

	unsigned int GetID() const { return m_ID;}

	void Update(unsigned int deltaTime);

private:
	unsigned int m_ID;
	float m_PosX;
	float m_PosY;
	float m_PosZ;
protected:
	typedef std::vector<CBaseComponent*>::iterator COMPITERATOR;
	std::vector<CBaseComponent*> m_Components;
};

#endif