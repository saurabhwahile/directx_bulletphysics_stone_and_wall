#ifndef BULLETPHYSICS_H
#define BULLETPHYSICS_H
#include <btBulletDynamicsCommon.h>
#include "Objects.h"

//variables for to bullet physics API
extern btAlignedObjectArray<btCollisionShape*>	m_collisionShapes; //keep the collision shapes, for deletion/cleanup
extern btBroadphaseInterface*					m_broadphase;
extern btCollisionDispatcher*					m_dispatcher;
extern btConstraintSolver*						m_solver;
extern btDefaultCollisionConfiguration*		m_collisionConfiguration;
extern btDynamicsWorld*						m_dynamicsWorld; //this is the most important class

void StepBulletPhysics();
void InitBulletPhysics();
void ExitBulletPhysics();

#endif