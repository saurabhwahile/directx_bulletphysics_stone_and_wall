#include "BulletPhysics.h"

//variables for to bullet physics API
btAlignedObjectArray<btCollisionShape*>	m_collisionShapes; //keep the collision shapes, for deletion/cleanup
btBroadphaseInterface*					m_broadphase;
btCollisionDispatcher*					m_dispatcher;
btConstraintSolver*						m_solver;
btDefaultCollisionConfiguration*		m_collisionConfiguration;
btDynamicsWorld*						m_dynamicsWorld; //this is the most important class

void StepBulletPhysics()
{
	if(m_dynamicsWorld)//step the simulation
		m_dynamicsWorld->stepSimulation(1.0f/60.0f);
}

void InitBulletPhysics()
{
	m_collisionConfiguration = new btDefaultCollisionConfiguration(); //collision configuration contains default setup for memory, collision setup
	m_dispatcher			 = new btCollisionDispatcher(m_collisionConfiguration); //use the default collision dispatcher 
	m_broadphase		  	 = new btDbvtBroadphase();
	m_solver			 	 = new btSequentialImpulseConstraintSolver; //the default constraint solver
	m_dynamicsWorld			 = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);
	m_dynamicsWorld->setGravity(btVector3(0,-20,0));

	g_fPosX  = 0.0f;
	g_fPosY  = -5.0f;
	g_fPosZ  = 0.0f;

	Objects Object;  //This Order Must Be Preserved
	Object.createGround(-3.0f);
	Object.createWallZ(5, 10, 2.0, 0.0f, 0.0f, 0.0f);
	Object.createStones(50.0f, 0.0f, 10.0f, 1);
}

void ExitBulletPhysics()
{
	//cleanup in the reverse order of creation/initialization

	for (int i=m_dynamicsWorld->getNumCollisionObjects()-1; i>=0; i--) //remove the rigidbodies from the dynamics world and delete them
	{
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if(body && body->getMotionState())
			delete body->getMotionState();

		m_dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}


	for (int j=0; j<m_collisionShapes.size(); j++) //delete collision shapes
	{
		btCollisionShape* shape = m_collisionShapes[j];
		delete shape;
	}

	m_collisionShapes.clear();
	delete m_dynamicsWorld;
	delete m_solver;
	delete m_broadphase;
	delete m_dispatcher;
	delete m_collisionConfiguration;
}