#pragma once
#include "BulletPhysics.h"
#include "Renderer.h"

extern float  g_fPosX;
extern float  g_fPosY;
extern float  g_fPosZ;

struct Counters
{
	int start;
	int end;
};

class Objects
{
public:
	static Counters TotalObjectsCounter;
	static Counters WallObjectsCounter;
	static Counters StoneObjectsCounter;
	Objects(void);
	void createGround(float depth);
	void createWallZ(int row, int column,float size, float posx, float posy, float posz);
	void createStones(float posx, float posy, float posz, float NoOfStones);
	void displayWallZ();
	void displayStone();

	void ApplyForce(btRigidBody* body, float force);

	~Objects(void);
};
