#include "Objects.h"

Counters Objects::StoneObjectsCounter;
Counters Objects::WallObjectsCounter;
Counters Objects::TotalObjectsCounter;

float  g_fPosX;
float  g_fPosY;
float  g_fPosZ;

Objects::Objects(void)
{
}

//Creating a static shape which will act as ground
void Objects::createGround(float depth)
{
	TotalObjectsCounter.start=0;
	TotalObjectsCounter.end=0;
	btCollisionShape* groundShape = new btBoxShape(btVector3(500,2,500));
	// btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),50);
	m_collisionShapes.push_back(groundShape);

	btScalar mass = 0; //rigidbody is static if mass is zero, otherwise dynamic
	btVector3 localInertia(0,0,0);

	groundShape->calculateLocalInertia(mass,localInertia);

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0,depth,0));

	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform); //motionstate provides interpolation capabilities, and only synchronizes 'active' objects
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,groundShape,localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	m_dynamicsWorld->addRigidBody(body); //add the body to the dynamics world
	TotalObjectsCounter.end++;
}

void Objects::createWallZ(int row, int column,float size, float posx, float posy, float posz)
{
	WallObjectsCounter.start=TotalObjectsCounter.end;
	WallObjectsCounter.end=TotalObjectsCounter.end;

	for(float i=0; i<row*size; i=i+size)
	{
		for(float j=0; j<column*size; j=j+size)
		{
			btCollisionShape* boxShape = new btBoxShape(btVector3(size/2 ,size/2 ,size/2 ));
			m_collisionShapes.push_back(boxShape);

			btScalar mass = 1.0f;
			btVector3 localInertia(0,0,0);

			boxShape->calculateLocalInertia(mass,localInertia);

			btTransform startTransform;
			startTransform.setIdentity();

			startTransform.setOrigin(btVector3( posx , posy+i ,posz+j ));

			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,boxShape,localInertia);
			btRigidBody* body = new btRigidBody(rbInfo);

			body->setFriction(0.1f);

			m_dynamicsWorld->addRigidBody(body);
			WallObjectsCounter.end++;
			TotalObjectsCounter.end++;
		}
	}

	// create the vertices using the Vertex struct
	Vertex vertices[] =
	{
		{ -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, },    // side 1
		{ 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, },
		{ -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, },
		{ 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, },

		{ -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, },    // side 2
		{ -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, },
		{ 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, },
		{ 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, },

		{ -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, },    // side 3
		{ -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, },
		{ 1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, },
		{ 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, },

		{ -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, },    // side 4
		{ 1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, },
		{ -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, },
		{ 1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, },

		{ 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, },    // side 5
		{ 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, },
		{ 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, },
		{ 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, },

		{ -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, },    // side 6
		{ -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, },
		{ -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, },
		{ -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, },
	};

	// create a vertex buffer interface called v_buffer_wall
	d3ddev->CreateVertexBuffer(24*sizeof(Vertex),
		0,
		CUSTOMFVF,
		D3DPOOL_MANAGED,
		&v_buffer_wall,
		NULL);

	VOID* pVoid;    // a void pointer

	// lock v_buffer_wall and load the vertices into it
	v_buffer_wall->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, vertices, sizeof(vertices));
	v_buffer_wall->Unlock();

	// create the indices using an int array
	short indices[] =
	{
		0, 1, 2,    // side 1
		2, 1, 3,
		4, 5, 6,    // side 2
		6, 5, 7,
		8, 9, 10,    // side 3
		10, 9, 11,
		12, 13, 14,    // side 4
		14, 13, 15,
		16, 17, 18,    // side 5
		18, 17, 19,
		20, 21, 22,    // side 6
		22, 21, 23,
	};

	// create an index buffer interface called i_buffer_wall
	d3ddev->CreateIndexBuffer(36*sizeof(short),
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&i_buffer_wall,
		NULL);

	// lock i_buffer_wall and load the indices into it
	i_buffer_wall->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, indices, sizeof(indices));
	i_buffer_wall->Unlock();
}

void Objects::createStones(float posx, float posy, float posz, float NoOfStones)
{
	StoneObjectsCounter.start=TotalObjectsCounter.end;
	StoneObjectsCounter.end=TotalObjectsCounter.end;
	for(float i=0; i<1.5f*NoOfStones; i=i+1.5f)
	{
		btTriangleMesh *mTriMesh = new btTriangleMesh();

		mTriMesh->addTriangle(
			btVector3( 0, 2, 0 ),
			btVector3( 0, 0, -2 ),
			btVector3( 2, 0, 0 )
			);
		mTriMesh->addTriangle(
			btVector3( 0, 2, 0 ),
			btVector3( -2, 0, 0 ),
			btVector3( 0, 0, -2 )
			);
		mTriMesh->addTriangle(
			btVector3( 0, 2, 0 ),
			btVector3( 0, 0, 2 ),
			btVector3( -2, 0, 0 )
			);
		mTriMesh->addTriangle(
			btVector3( 0, 2, 0 ),
			btVector3( 2, 0, 0 ),
			btVector3( 0, 0, 2 )
			);
		mTriMesh->addTriangle(
			btVector3( 0, -2, 0 ),
			btVector3( 2, 0, 0 ),
			btVector3( 0, 0, -2 )
			);
		mTriMesh->addTriangle(
			btVector3( 0, -2, 0 ),
			btVector3( 0, 0, -2 ),
			btVector3( -2, 0, 0 )
			);
		mTriMesh->addTriangle(
			btVector3( 0, -2, 0 ),
			btVector3( -2, 0, 0 ),
			btVector3( 0, 0, 2 )
			);
		mTriMesh->addTriangle(
			btVector3( 0, -2, 0 ),
			btVector3( 0, 0, 2 ),
			btVector3( 2, 0, 0 )
			);

		btCollisionShape *mTriMeshShape = new btSphereShape(1.5f);//new btBvhTriangleMeshShape(mTriMesh, true);
		m_collisionShapes.push_back(mTriMeshShape);

		btScalar mass = 1.0f;
		btVector3 localInertia( 0.0f, 0.0f , 0.0f);

		mTriMeshShape->calculateLocalInertia(mass,localInertia);

		btTransform startTransform;
		startTransform.setIdentity();

		startTransform.setOrigin(btVector3( posx , posy ,posz+i ));

		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,mTriMeshShape,localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		m_dynamicsWorld->addRigidBody(body);
		StoneObjectsCounter.end++;
		TotalObjectsCounter.end++;
	}
	Vertex vertices[] =
	{
		{ 0.0f, 2.0f, 0.0f, 1.0f, 1.0f, -1.0f, },
		{ 0.0f, 0.0f, -2.0f, 1.0f, 1.0f, -1.0f, },
		{ 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, -1.0f, },

		{ 0.0f, 2.0f, 0.0f, -1.0f, 1.0f, -1.0f, },
		{ -2.0f, 0.0f, 0.0f, -1.0f, 1.0f, -1.0f, },
		{ 0.0f, 0.0f, -2.0f, -1.0f, 1.0f, -1.0f, },

		{ 0.0f, 2.0f, 0.0f, -1.0f, 1.0f, 1.0f, },
		{ 0.0f, 0.0f, 2.0f, -1.0f, 1.0f, 1.0f, },
		{ -2.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, },

		{ 0.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f, },
		{ 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, },
		{ 0.0f, 0.0f, 2.0f, 1.0f, 1.0f, 1.0f, },

		{ 0.0f, -2.0f, 0.0f, 1.0f, -1.0f, -1.0f, },
		{ 2.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, },
		{ 0.0f, 0.0f, -2.0f, 1.0f, -1.0f, -1.0f, },

		{ 0.0f, -2.0f, 0.0f, -1.0f, -1.0f, -1.0f, },
		{ 0.0f, 0.0f, -2.0f, -1.0f, -1.0f, -1.0f, },
		{ -2.0f, 0.0f, 0.0f, -1.0f, -1.0f, -1.0f, },

		{ 0.0f, -2.0f, 0.0f, -1.0f, -1.0f, 1.0f, },
		{ -2.0f, 0.0f, 0.0f, -1.0f, -1.0f, 1.0f, },
		{ 0.0f, 0.0f, 2.0f, -1.0f, -1.0f, 1.0f, },

		{ 0.0f, -2.0f, 0.0f, 1.0f, -1.0f, 1.0f, },
		{ 0.0f, 0.0f, 2.0f, 1.0f, -1.0f, 1.0f, },
		{ 2.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, },
	};

	// create a vertex buffer interface called v_buffer_stone
	d3ddev->CreateVertexBuffer(48*sizeof(Vertex),
		0,
		CUSTOMFVF,
		D3DPOOL_MANAGED,
		&v_buffer_stone,
		NULL);

	VOID* pVoid;    // a void pointer

	// lock v_buffer_stone and load the vertices into it
	v_buffer_stone->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, vertices, sizeof(vertices));
	v_buffer_stone->Unlock();

	// create the indices using an int array
	short indices[] =
	{
		0, 1, 2,    
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,
		12, 13, 14,
		15, 16, 17,
		18, 19, 20,
		21, 22, 23
	};

	// create an index buffer interface called i_buffer_stone
	d3ddev->CreateIndexBuffer(24*sizeof(short),
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&i_buffer_stone,
		NULL);

	// lock i_buffer_wall and load the indices into it
	i_buffer_stone->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, indices, sizeof(indices));
	i_buffer_stone->Unlock();
}

void Objects::displayWallZ()
{
	for(int i=WallObjectsCounter.start; i<WallObjectsCounter.end; i++)
	{
		btCollisionObject*	colObj=m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody*		body=btRigidBody::upcast(colObj);

		body->getCollisionShape();
		if(body && body->getMotionState())
		{
			btTransform trans;
			btDefaultMotionState* myMotionState = (btDefaultMotionState*)body->getMotionState();
			myMotionState->getWorldTransform(trans);

			btQuaternion rot=myMotionState->m_graphicsWorldTrans.getRotation();
			btVector3 axisPhysics=rot.getAxis();
			D3DXMATRIX matPhysicsRotate;
			D3DXVECTOR3 *axis=new D3DXVECTOR3(axisPhysics.getX(),axisPhysics.getY(),axisPhysics.getZ());
			D3DXMatrixRotationAxis(&matPhysicsRotate, axis, rot.getAngle());
			delete axis;

			D3DXMATRIX matPhysicsTranslate;

			D3DXMatrixTranslation(&matPhysicsTranslate, trans.getOrigin().getX() , trans.getOrigin().getY(), trans.getOrigin().getZ());

			// select the vertex and index buffers to use
			d3ddev->SetStreamSource(0, v_buffer_wall, 0, sizeof(Vertex));
			d3ddev->SetIndices(i_buffer_wall);

			d3ddev->SetTransform(D3DTS_WORLD, &( matPhysicsRotate * matPhysicsTranslate ));

			// draw the cube
			d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12); 
		}
	}
}

void Objects::displayStone()
{
	for(int i=StoneObjectsCounter.start;i<StoneObjectsCounter.end;i++)
	{
		btCollisionObject*	colObj=m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody*		body=btRigidBody::upcast(colObj);

		body->getCollisionShape();

		ApplyForce(body, 100);

		if(body && body->getMotionState())
		{
			btTransform trans;
			btDefaultMotionState* myMotionState = (btDefaultMotionState*)body->getMotionState();
			myMotionState->getWorldTransform(trans);

			btQuaternion rot=myMotionState->m_graphicsWorldTrans.getRotation();
			btVector3 axisPhysics=rot.getAxis();
			D3DXMATRIX matPhysicsRotate;
			D3DXVECTOR3 *axis=new D3DXVECTOR3(axisPhysics.getX(),axisPhysics.getY(),axisPhysics.getZ());
			D3DXMatrixRotationAxis(&matPhysicsRotate, axis, rot.getAngle());
			delete axis;

			D3DXMATRIX matPhysicsTranslate;

			D3DXMatrixTranslation(&matPhysicsTranslate, trans.getOrigin().getX() , trans.getOrigin().getY(), trans.getOrigin().getZ());

			// select the vertex and index buffers to use
			d3ddev->SetStreamSource(0, v_buffer_stone, 0, sizeof(Vertex));
			d3ddev->SetIndices(i_buffer_stone);

			d3ddev->SetTransform(D3DTS_WORLD, &( matPhysicsRotate * matPhysicsTranslate ));

			// draw the cube
			d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 8); 
		}
	}
}


void Objects::ApplyForce(btRigidBody* body, float force)
{

	if(GetAsyncKeyState(VK_UP))
		body->applyForce(btVector3( 0, 0 ,-force ), btVector3( 0, 0 ,0 ));

	if(GetAsyncKeyState(VK_DOWN))
		body->applyForce(btVector3( 0, 0 ,force ), btVector3( 0, 0 ,0 ));

	if(GetAsyncKeyState(VK_LEFT))	
		body->applyForce(btVector3( force, 0 ,0 ), btVector3( 0, 0 ,0 ));

	if(GetAsyncKeyState(VK_RIGHT))
		body->applyForce(btVector3( -force, 0 ,0 ), btVector3( 0, 0 ,0 ));

	if(GetAsyncKeyState('A'))
		body->applyForce(btVector3( 0, force ,0 ), btVector3( 0, 0 ,0 ));

	if(GetAsyncKeyState('S'))
		body->applyForce(btVector3( 0, -force ,0 ), btVector3( 0, 0 ,0 ));

}

Objects::~Objects(void)
{
}
