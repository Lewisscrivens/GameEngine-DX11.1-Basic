////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

////////////////////////////////////////////////////////////////////////////////
// CONTROLES:
// 
// Fullscreen: F11 [NOT WORKING CURRENTLY]
// Camera Up: Mouse y-axis
// Camera Right: Mouse x-axis
// Forward: W
// Back: S
// Left: A
// Right: D
// Up: Space Bar
// Down: Left Ctrl
//
// Moon Orbit Speed:
// Increase: NUM_PLUS
// Decrease: NUM_MINUS
// Reset: NUM_0
//
// Moon Eclipse:
// Enable: 0
// Disable: 9
// 
// Increase Camera Speed: Left Shift
// Reset Camera Location: R
// Set Camera Start Location: H
//
// Launch Shuttle: M
// Increase Shuttle Speed: PgUp
// Decrease Shuttle Speed: PgDn
//
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////
// GLOBALS ////////////
///////////////////////

static bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 4000.0f;//Made bigger as the skybox wouldnt render etc.
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

	float moonOrbitSpeed;
	float moonOrbitSpeedOriginal;

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
	ModelClass* m_Sun;
	BumpModelClass* m_Earth;
	BumpModelClass* m_Moon;
	BumpModelClass* m_Shuttle;
	BumpModelClass* m_Satalite;
	BumpModelClass* m_UFO;

	
	float shuttleDistance;
	float shuttleSpeed;
	float shuttleMaxSpeed;
	float shuttleDefaultSpeed;
	float shuttleFaceMoon;
	bool shuttleTraveling;

	float moonXTranslation;
	float moonRotationY;
	bool moonEclipse;
	
	double radianPerDegree;
};

#endif