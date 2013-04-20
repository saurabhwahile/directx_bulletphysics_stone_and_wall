#ifndef RENDERER_H
#define RENDERER_H

#include <d3d9.h>
#include <d3dx9.h>

#include "BulletPhysics.h"

// define the screen resolution
#define SCREEN_WIDTH  1366
#define SCREEN_HEIGHT 768

//#define SCREEN_WIDTH  1024
//#define SCREEN_HEIGHT 768

#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_NORMAL )

// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

extern LPDIRECT3D9 d3d; // the pointer to our Direct3D interface
extern LPDIRECT3DDEVICE9 d3ddev; // the pointer to the device class
extern LPDIRECT3DVERTEXBUFFER9 v_buffer_wall;
extern LPDIRECT3DINDEXBUFFER9 i_buffer_wall;
extern LPDIRECT3DVERTEXBUFFER9 v_buffer_stone;
extern LPDIRECT3DINDEXBUFFER9 i_buffer_stone;

struct Vertex
{
    FLOAT x, y, z;    // from the D3DFVF_XYZ flag
    D3DVECTOR NORMAL; //from the D3DFVF_NORMAL flag
};

// function prototypes

void init_graphics(void);
void init_light(void);
void initD3D(HWND hWnd); // sets up and initializes Direct3D
void render_frame(void); // renders a single frame
void cleanD3D(void); // closes Direct3D and releases memory
void Render_This();

#endif