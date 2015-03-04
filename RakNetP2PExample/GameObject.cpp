#include "GameObject.h"
#include <iostream>
#include <stdio.h>
#include "../GameExample/BaseComponent.h"

GameObject::GameObject() : m_ID(0)
	,m_PosX(0.0f)
	,m_PosY(0.0f)
	,m_PosZ(0.0f)
{

}

void GameObject::GetPosition(float &outX,float &outY, float &outZ ) const
{
	outX = m_PosX;
	outY = m_PosY;
	outZ = m_PosZ;
}

void GameObject::SetPosition(float x, float y, float z)
{
	m_PosX = x;
	m_PosY = y;
	m_PosZ = z;
}

void GameObject::PrintPosition() const
{
	printf("My Position\nX:%.2f \ny:%.2f\nz:%.2f\n ",m_PosX,m_PosY,m_PosZ);
}

void GameObject::Update(unsigned int deltaTime)
{
	COMPITERATOR it = m_Components.begin();
	for(it;it != m_Components.end(); ++it)
	{
		(*it)->Update(deltaTime);
	}
}