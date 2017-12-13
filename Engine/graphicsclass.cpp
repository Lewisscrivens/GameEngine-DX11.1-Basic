////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_Input = 0;
	m_D3D = 0;
	m_Timer = 0;
	m_ShaderManager = 0;
	m_Light = 0;
	m_Position = 0;
	m_Camera = 0;
	m_Model1 = 0;
	m_Model2 = 0;
	m_Model3 = 0;
	m_Skybox = 0;
	m_Earth = 0;
	m_Moon = 0;

	radianPerDegree = 0.0174533; //1 degree equlas this value

	startX = -100.0f;
	startY = 0.0f;
	startZ = -200.0f;

	startRotX = 0.0f;
	startRotY = 30.0f * radianPerDegree;
	startRotZ = 0.0f;

	mouseSensitivity = 0.0f;
	moveSpeed = 0.0f;

	moonOrbitSpeed = 0.05; // The bigger the float the faster the moon gets as the rotation amount becomes greater.
	moonOrbitSpeedOriginal = moonOrbitSpeed;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result;

	// Create the input object.  The input object will be used to handle reading the keyboard and mouse input from the user.
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	result = m_Input->Initialize(hinstance, hwnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the shader manager object.
	m_ShaderManager = new ShaderManagerClass;
	if(!m_ShaderManager)
	{
		return false;
	}

	// Initialize the shader manager object.
	result = m_ShaderManager->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the shader manager object.", L"Error", MB_OK);
		return false;
	}

	// Create the timer object.
	m_Timer = new TimerClass;
	if (!m_Timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Timer->Initialize();
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}

	// Create the position object.
	m_Position = new PositionClass;
	if (!m_Position)
	{
		return false;
	}

	// Set the initial position and rotation of the viewer.
	m_Position->SetPosition(startX, startY, startZ);
	m_Position->SetRotation(startRotX, startRotY, startRotZ);

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	//m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, 0.0f, 0.0f);//Direction vector for the light source.
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(64.0f);

	// Create the model object.
	m_Model1 = new ModelClass;
	if(!m_Model1)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model1->Initialize(m_D3D->GetDevice(), "../Engine/data/cube.txt", L"../Engine/data/marble.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the first model object.", L"Error", MB_OK);
		return false;
	}

	// Create the second model object.
	m_Model2 = new ModelClass;
	if(!m_Model2)
	{
		return false;
	}

	// Initialize the second model object.
	result = m_Model2->Initialize(m_D3D->GetDevice(), "../Engine/data/new-ninjaHead.txt", L"../Engine/data/metal.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the second model object.", L"Error", MB_OK);
		return false;
	}

	// Create the third bump model object for models with normal maps and related vectors.
	m_Model3 = new BumpModelClass;
	if (!m_Model3)
	{
		return false;
	}

	// Initialize the bump model object.
	result = m_Model3->Initialize(m_D3D->GetDevice(), "../Engine/data/cube.txt", L"../Engine/data/stone.dds",
		L"../Engine/data/normal.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the third model object.", L"Error", MB_OK);
		return false;
	}

	// Create the Skybox object.
	m_Skybox = new ModelClass;
	if (!m_Skybox)
	{
		return false;
	}

	// Initialize the second model object.
	result = m_Skybox->Initialize(m_D3D->GetDevice(), "../Engine/data/Sphere.txt", L"../Engine/data/Skybox.dds");
	if (!m_Skybox)
	{
		MessageBox(hwnd, L"Could not initialize the TropicalIsland object.", L"Error", MB_OK);
		return false;
	}

	// Create the Earth object.
	m_Earth = new BumpModelClass;
	if (!m_Earth)
	{
		return false;
	}

	// Initialize the second model object.
	result = m_Earth->Initialize(m_D3D->GetDevice(), "../Engine/data/SphereHQ.txt", L"../Engine/data/Earth.dds",
					L"../Engine/data/EarthNormal.dds");
	if (!m_Earth)
	{
		MessageBox(hwnd, L"Could not initialize the Earth.", L"Error", MB_OK);
		return false;
	}

	// Create the moon object for models with normal maps and related vectors.
	m_Moon = new BumpModelClass;
	if (!m_Moon)
	{
		return false;
	}

	// Initialize the bump model object.
	result = m_Moon->Initialize(m_D3D->GetDevice(), "../Engine/data/SphereHQ.txt", L"../Engine/data/Moon.dds",
		L"../Engine/data/MoonNormal.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the moon object.", L"Error", MB_OK);
		return false;
	}


	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the model objects.
	if(m_Model1)
	{
		m_Model1->Shutdown();
		delete m_Model1;
		m_Model1 = 0;
	}

	if(m_Model2)
	{
		m_Model2->Shutdown();
		delete m_Model2;
		m_Model2 = 0;
	}

	if (m_Model3)
	{
		m_Model3->Shutdown();
		delete m_Model3;
		m_Model3 = 0;
	}

	if (m_Skybox)
	{
		m_Skybox->Shutdown();
		delete m_Skybox;
		m_Skybox = 0;
	}

	if (m_Earth)
	{
		m_Earth->Shutdown();
		delete m_Earth;
		m_Earth = 0;
	}

	if (m_Moon)
	{
		m_Moon->Shutdown();
		delete m_Moon;
		m_Moon = 0;
	}

	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the position object.
	if (m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

	// Release the shader manager object.
	if(m_ShaderManager)
	{
		m_ShaderManager->Shutdown();
		delete m_ShaderManager;
		m_ShaderManager = 0;
	}

	// Release the timer object.
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	// Release the input object.
	if (m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	return;
}


bool GraphicsClass::Frame()
{
	bool result;

	// Update the system stats.
	m_Timer->Frame();

	// Read the user input.
	result = m_Input->Frame();
	if (!result)
	{
		return false;
	}

	if (m_Input->IsHPressed())
	{
		m_Position->GetPosition(startX, startY, startZ);//Gets the position and sets it to the varibales that are for resetting the camera position.
		m_Position->GetRotation(startRotX, startRotY, startRotZ);//Gets the rotation and sets it to the varibales that are for resetting the camera rotation.
	}

	// Check if the user pressed escape and wants to exit the application.
	if (m_Input->IsEscapePressed() == true)
	{
		return false;
	}

	//If R key is pressed then run the resetCameraPosition method.
	if (m_Input->IsRPressed() == true)
	{
		ResetCameraPosition();
	}

	// Do the frame input processing.
	result = HandleMovementInput(m_Timer->GetTime());
	if (!result)
	{
		return false;
	}

	// Render the graphics.
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::HandleMovementInput(float frameTime)
{
	bool keyDown;
	float posX, posY, posZ, rotX, rotY, rotZ;
	int mousePosX, mousePosY, mouseChangeX, mouseChangeY;

	//Gets the mouseChange between frames for the X axis.
	mouseChangeX = m_Input->GetMouseXChange();
	//Gets the mouseChange between frames for the Y axis.
	mouseChangeY = m_Input->GetMouseYChange();

	mouseSensitivity = 0.1f;//Added this so that the mouseSensitivity can be changed easily.

	moveSpeed = 2.0f;//Movement speed is now multiplied by two.

	// Set the frame time for calculating the updated position.
	m_Position->SetFrameTime(frameTime);

	// Handle the input.
	keyDown = m_Input->IsUpPressed();
	m_Position->LookUpward(keyDown);

	keyDown = m_Input->IsDownPressed();
	m_Position->LookDownward(keyDown);

	keyDown = m_Input->IsLeftPressed();
	m_Position->TurnLeft(keyDown);

	keyDown = m_Input->IsRightPressed();
	m_Position->TurnRight(keyDown);

	keyDown = m_Input->IsWPressed();
	m_Position->MoveForward(keyDown);

	keyDown = m_Input->IsSPressed();
	m_Position->MoveBackward(keyDown);

	keyDown = m_Input->IsAPressed();
	m_Position->MoveLeft(keyDown);

	keyDown = m_Input->IsDPressed();
	m_Position->MoveRight(keyDown);

	keyDown = m_Input->IsSpaceBarPressed();
	m_Position->MoveUpward(keyDown);

	keyDown = m_Input->IsLeftCtrlPressed();
	m_Position->MoveDownward(keyDown);

	keyDown = m_Input->IsLeftShiftPressed();
	m_Position->SetMoveSpeed(keyDown, moveSpeed);

	keyDown = m_Input->IsPlusPressed();//If the numpad plus key is pressed then the moonOrbitSpeed is increased.
	if (keyDown)
	{
		moonOrbitSpeed += 0.001;
	}

	keyDown = m_Input->IsMinusPressed();//If the numpad minus key is pressed then the moonOrbitSpeed is decreased.
	if (keyDown)
	{
		moonOrbitSpeed -= 0.001;
	}

	keyDown = m_Input->IsNum0Pressed();//If the numpad 0 key is pressed then the moonOrbitSpeed is reset.
	if (keyDown && moonOrbitSpeed != moonOrbitSpeedOriginal)//Checks if the speed has already been reset already.
	{
		moonOrbitSpeed = moonOrbitSpeedOriginal;
	}

	//Run the poisition method move mouse and pass in variables from the input class.
	m_Position->MoveMouse(mouseChangeX, mouseChangeY, mouseSensitivity);

	// Get the view point position/rotation.
	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);

	//Reset the cursor to the centre of the screen as we now have implemented mouse movement.
	m_Input->ResetMousePosition();

	// Set the position of the camera.
	m_Camera->SetPosition(posX, posY, posZ);
	m_Camera->SetRotation(rotX, rotY, rotZ);

	return true;
}

bool GraphicsClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, translateMatrix;
	bool result;

	static float rotation = 0.0f;

	// Update the rotation variable each frame.
	rotation += (float)XM_PI * 0.0005f * m_Timer->GetTime();

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Setup the rotation and translation of the first model.
	worldMatrix = XMMatrixRotationZ(rotation * 3.0f);
	translateMatrix = XMMatrixTranslation(- 3.5f, 0.0f, 0.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, translateMatrix);

	// Render the first model using the texture shader.
	m_Model1->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderTextureShader(m_D3D->GetDeviceContext(), m_Model1->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
	m_Model1->GetTexture());

	if(!result)
	{
		return false;
	}

	// Setup the rotation and translation of the second model.
	m_D3D->GetWorldMatrix(worldMatrix);
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixScaling(0.03f, 0.03f, 0.03f));
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationY(rotation));
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixTranslation(0.0f, 0.0f, 0.0f));

	// Render the second model using the light shader.
	m_Model2->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model2->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
									   m_Model2->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), 
									   m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if(!result)
	{
		return false;
	}

	//Set rotation, translation and scale for the moon
	m_D3D->GetWorldMatrix(worldMatrix);
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixScaling(0.03f, 0.03f, 0.03f));
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationY(rotation));
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixTranslation(3.5f, 0.0f, 0.0f));

	// Render the third model using the bump map shader.
	m_Model3->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderBumpMapShader(m_D3D->GetDeviceContext(), m_Model3->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
	m_Model3->GetColorTexture(), m_Model3->GetNormalMapTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor());

	if (!result)
	{
		return false;
	}

	// Setup the rotation and translation of the Skybox.
	m_D3D->GetWorldMatrix(worldMatrix);
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixScaling(-100.0f, -100.0f, -100.0f));//Revered the scale so that the sphere was turned inside out so the texture renders on the inside.
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationY(0.0f));
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixTranslation(0.0f, 0.0f, 0.0f));

	// Render the Skybox using the light shader.
	m_Skybox->Render(m_D3D->GetDeviceContext());
	m_ShaderManager->RenderTextureShader(m_D3D->GetDeviceContext(), m_Skybox->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
	m_Skybox->GetTexture());

	if (!result)
	{
		return false;
	}



	// Setup the rotation and translation of the Earth.
	m_D3D->GetWorldMatrix(worldMatrix);
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixScaling(0.5f, 0.5f, 0.5f));
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationY(rotation / 4));
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationZ(23.5f * radianPerDegree));//The earth is on a 23.5 degree axis so I have created a variable radian which is equal to the amount of radians in 1 degree.
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixTranslation(0.0f, 0.0f, 0.0f));

	// Render the Earth using the bump map shader.
	m_Earth->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderBumpMapShader(m_D3D->GetDeviceContext(), m_Earth->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
	m_Earth->GetColorTexture(), m_Earth->GetNormalMapTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor());

	if (!result)
	{
		return false;
	}


	//Set rotation, translation and scale for the moon
	m_D3D->GetWorldMatrix(worldMatrix);
	XMMATRIX moonRot;
	XMMATRIX moonTran;
	XMMATRIX moonScale;
	moonScale = XMMatrixMultiply(worldMatrix, XMMatrixScaling(0.2f, 0.2f, 0.2f));//Scaling of the moon (x,y,z).
	moonRot = XMMatrixMultiply(worldMatrix, XMMatrixRotationY(rotation * moonOrbitSpeed));//Rotation around YAW (y-axis), rotation is a constantly incrementing float value.
	moonTran = XMMatrixMultiply(worldMatrix, XMMatrixTranslation(-500.0f, 0.0f, 0.0f));//Positional values (x,y,x).
	worldMatrix = XMMatrixMultiply(worldMatrix, moonScale * moonTran * moonRot);//Add the matrixes together by multiplication and then add them to the moons world matrix by again multiplication.

	// Render moon using the bump map shader.
	m_Moon->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderBumpMapShader(m_D3D->GetDeviceContext(), m_Moon->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Moon->GetColorTexture(), m_Moon->GetNormalMapTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor());

	if (!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();


	return true;
}

void GraphicsClass::ResetCameraPosition()
{
	// Reset the camera to the original position...
	m_Position->SetPosition(startX, startY, startZ);
	m_Position->SetRotation(startRotX, startRotY, startRotZ);
}


