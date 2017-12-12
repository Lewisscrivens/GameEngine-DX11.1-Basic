////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

////////////////////////////////////////////////////////////////////////////////
// CONTROLES:
// 
// Fullscreen: F11
// Camera Up: Mouse y-axis
// Camera Right: Mouse x-axis
// Forward: W
// Back: S
// Left: A
// Right: D
// Up: Space Bar
// Down: Left Ctrl
// 
// Increase Speed: Left Shift
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////
// GLOBALS ////////////
///////////////////////

static bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 2000.0f;//Doubled so the skybox still renders at greater distances.
const float SCREEN_NEAR = 0.1f;



///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "inputclass.h"
#include "d3dclass.h"
#include "timerclass.h"
#include "shadermanagerclass.h"
#include "positionclass.h"
#include "cameraclass.h"
#include "lightclass.h"
#include "modelclass.h"
#include "bumpmodelclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();
	void ResetCameraPosition();


private:
	//bool Render(float);
	//Xu
	bool HandleMovementInput(float);
	bool Render();

	

public:

	float startX;
	float startY;
	float startZ;

	float startRotX;
	float startRotY;
	float startRotZ;

	float mouseSensitivity;
	float moveSpeed;

private:
	InputClass* m_Input;
	D3DClass* m_D3D;
	TimerClass* m_Timer;
	ShaderManagerClass* m_ShaderManager;
	PositionClass* m_Position;
	CameraClass* m_Camera;
	LightClass* m_Light;
	ModelClass* m_Model1;
	ModelClass* m_Model2;
	BumpModelClass* m_Model3;
	ModelClass* m_Skybox;

};

#endif