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
	m_Sun = 0;
	m_Earth = 0;
	m_Moon = 0;
	m_Shuttle = 0;
	m_Satelite = 0;
	m_UFO = 0;

	// Variables for controlling the shuttle behaviour.
	shuttleDistance = 0.0f;
	shuttleSpeed = 0.5f;
	shuttleMaxSpeed = 2.5f;
	shuttleRotation = 0.0f;
	shuttleDefaultSpeed = shuttleSpeed;
	shuttleTraveling = false;
	shuttleFaceMoon = true;

	// 1 degree equlas this value (Quickest method as it doesnt have to be 100% accurate for my use)
	radianPerDegree = 0.0174533;

	// Current position variable instalisation.
	posX = 0;
	posY = 0;
	posZ = 0;

	// Starting position for the camera on instalisation.
	startX = -100.0f;
	startY = 0.0f;
	startZ = -200.0f;

	startRotX = 0.0f;
	startRotY = 30.0f * radianPerDegree;
	startRotZ = 0.0f;

	mouseSensitivity = 0.0f;
	moveSpeed = 0.0f;

	// Variables for controlling the moon orbit.
	moonOrbitSpeed = 0.001f;
	moonOrbitSpeedOriginal = moonOrbitSpeed;
	moonXTranslation = -500.0f;// This controls both the translation along the x-axis of the moon and the distance for the shuttle to travel.
	moonRotationY = 0.0f;
	moonEclipse = false;
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
	// m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, 0.0f, 0.0f);// Direction vector for the light source.
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

	// Initialize the Skybox object.
	result = m_Skybox->Initialize(m_D3D->GetDevice(), "../Engine/data/Sphere.txt", L"../Engine/data/Skybox.dds");
	if (!m_Skybox)
	{
		MessageBox(hwnd, L"Could not initialize the Skybox object.", L"Error", MB_OK);
		return false;
	}

	// Create the Sun object.
	m_Sun = new ModelClass;
	if (!m_Sun)
	{
		return false;
	}

	// Initialize the Sun object.
	result = m_Sun->Initialize(m_D3D->GetDevice(), "../Engine/data/SphereHQ.txt", L"../Engine/data/Sun.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Sun object.", L"Error", MB_OK);
		return false;
	}

	// Create the Earth object.
	m_Earth = new BumpModelClass;
	if (!m_Earth)
	{
		return false;
	}

	// Initialize the Earth object.
	result = m_Earth->Initialize(m_D3D->GetDevice(), "../Engine/data/SphereHQ.txt", L"../Engine/data/Earth.dds",
					L"../Engine/data/EarthNormal.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Earth object.", L"Error", MB_OK);
		return false;
	}

	// Create the Moon object with normal texture.
	m_Moon = new BumpModelClass;
	if (!m_Moon)
	{
		return false;
	}

	// Initialize the Moon object.
	result = m_Moon->Initialize(m_D3D->GetDevice(), "../Engine/data/SphereHQ.txt", L"../Engine/data/Moon.dds",
		L"../Engine/data/MoonNormal.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Moon object.", L"Error", MB_OK);
		return false;
	}

	// Create the Shuttle object.
	m_Shuttle = new ModelClass;
	if (!m_Shuttle)
	{
		return false;
	}

	// Initialize the Shuttle object.
	result = m_Shuttle->Initialize(m_D3D->GetDevice(), "../Engine/data/Shuttle.txt", L"../Engine/data/metal.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the shuttle object.", L"Error", MB_OK);
		return false;
	}

	// Create the Satelite object.
	m_Satelite = new ModelClass;
	if (!m_Satelite)
	{
		return false;
	}

	// Initialize the Satelite object.
	result = m_Satelite->Initialize(m_D3D->GetDevice(), "../Engine/data/Satelite.txt", L"../Engine/data/metal.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Satelite object.", L"Error", MB_OK);
		return false;
	}

	// Create the UFO object.
	m_UFO = new ModelClass;
	if (!m_UFO)
	{
		return false;
	}

	// Initialize the UFO object.
	result = m_UFO->Initialize(m_D3D->GetDevice(), "../Engine/data/UFO.txt", L"../Engine/data/metal.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the UFO object.", L"Error", MB_OK);
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

	if (m_Sun)
	{
		m_Sun->Shutdown();
		delete m_Sun;
		m_Sun = 0;
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

	if (m_Shuttle)
	{
		m_Shuttle->Shutdown();
		delete m_Shuttle;
		m_Shuttle = 0;
	}

	if (m_Satelite)
	{
		m_Satelite->Shutdown();
		delete m_Satelite;
		m_Satelite = 0;
	}

	if (m_UFO)
	{
		m_UFO->Shutdown();
		delete m_UFO;
		m_UFO = 0;
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

	// CONTROLES (See .h file for list of controles):

	if (m_Input->IsHPressed())// Used to set current position as save position.
	{
		m_Position->GetPosition(startX, startY, startZ);// Gets the position and sets it to the varibales that are for the saved camera position.
		m_Position->GetRotation(startRotX, startRotY, startRotZ);// Gets the rotation and sets it to the varibales that are for the saved camera rotation.
	}

	// Check if the user pressed escape and wants to exit the application.
	if (m_Input->IsEscapePressed() == true)
	{
		return false;
	}

	// Check if the user has pressed R.
	if (m_Input->IsRPressed() == true)
	{
		SetCameraPosition(startX, startY, startZ, startRotX, startRotY, startRotZ);// Set camera position to stored position and rotation variables.
	}

	// Check if the user has pressed plus & moonOrbitSpeed is not more than 10x bigger than normal.
	if (m_Input->IsPlusPressed() && moonOrbitSpeed < moonOrbitSpeedOriginal * 10)
	{
		moonOrbitSpeed += 0.0001;// Increase the moonOrbitSpeed.
	}

	// Check if the user has pressed minus & the moonOrbitSpeed is bigger than original value.
	if (m_Input->IsMinusPressed() && moonOrbitSpeed > moonOrbitSpeedOriginal)
	{
		moonOrbitSpeed -= 0.0001;// Decrease the moonOrbitSpeed.
	}

	// Check if the user has pressed num_0 & moonOrbitSpeed doesnt already equal original value.
	if (m_Input->IsNum0Pressed() && moonOrbitSpeed != moonOrbitSpeedOriginal)
	{
		moonOrbitSpeed = moonOrbitSpeedOriginal;// Reset moonOrbitSpeed to original value.
	}

	//shuttleController:

	// Check if the user has pressed M & the shuttle isnt traveling OR if shuttle is traveling and the shuttle hasnt reached the moon or earth yet.
	if (m_Input->IsMPressed() && !shuttleTraveling || shuttleTraveling && shuttleDistance > moonXTranslation && shuttleDistance < 0)
	{
		shuttleTraveling = true;// Shuttle is now traveling.

		if (shuttleFaceMoon)// The shuttle is facing the moon.
		{
			shuttleDistance -= shuttleSpeed;//Decrement the shuttle distance.
		}
		else
		{
			shuttleDistance += shuttleSpeed;//Increment the shuttle distance.
		}
	}
	else if (shuttleTraveling)// If the shuttle is traveling but it has reached the moon or the earth.
	{
		shuttleTraveling = false;// Shuttle no longer traveling.

		if (shuttleDistance <= moonXTranslation)// If the shuttle is at the Moon translation.
		{
			shuttleFaceMoon = false;// Shuttle facing Earth.
			shuttleRotation = 180.0f * radianPerDegree;// Set shuttleRotation to 180 degrees to face the earth.
		}
		else if (shuttleDistance >= 0.0f)// If the shuttle is at the Earth translation.
		{
			shuttleFaceMoon = true;// Shuttle facing Moon.
			shuttleRotation = 0.0f;// Set shuttleRotation to 0 degrees to face the earth.
		}

		shuttleSpeed = shuttleDefaultSpeed;// Reset the shuttleSpeed back to original.
	}
	else if (!shuttleTraveling && shuttleSpeed != shuttleDefaultSpeed)// Check if the shuttle isnt traveling & the shuttleSpeed still hasnt been reset.
	{
		shuttleSpeed = shuttleDefaultSpeed;// Reset the shuttleSpeed back to original.
	}

	// Check if the user is pressing PgUp.
	if (m_Input->IsPgUpPressed())
	{
		if (shuttleFaceMoon && shuttleSpeed < shuttleMaxSpeed || !shuttleFaceMoon && shuttleSpeed > -shuttleMaxSpeed)// If the shuttleSpeed is smaller than max shuttle speed.
		{
			shuttleSpeed += 0.01;// Increase shuttleSpeed.
		}
	}

	// Check if the user is pressing PgDn.
	if (m_Input->IsPgDnPressed())
	{
		if (shuttleFaceMoon && shuttleSpeed > shuttleDefaultSpeed || !shuttleFaceMoon && shuttleSpeed > shuttleDefaultSpeed)// If the shuttleSpeed is larger than minimum shuttle speed which is shuttleDefaultSpeed.
		{
			shuttleSpeed -= 0.01;// Decrease shuttleSpeed.
		}
	}

	// Check if the user is pressing 0 & the moon isnt already in an eclipse.
	if (m_Input->Is0Pressed() && !moonEclipse)
	{
		moonEclipse = true;// Enable Eclipse.
	}

	// Check if the user is pressing 9 & the moon is in an eclipse.
	if (m_Input->Is9Pressed() && moonEclipse)
	{
		moonEclipse = false;// Disable Eclipse.
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
	float rotX, rotY, rotZ;//Position variables moved to .h class.
	int mousePosX, mousePosY, mouseChangeX, mouseChangeY;

	// Gets the mouseChange between frames for the X axis.
	mouseChangeX = m_Input->GetMouseXChange();
	// Gets the mouseChange between frames for the Y axis.
	mouseChangeY = m_Input->GetMouseYChange();

	mouseSensitivity = 0.1f;// Used to affect mouseMovement speed easily.

	moveSpeed = 2.0f;// Movement speed variable. Set to 2x faster.

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

	// Run the poisition method moveMouse and pass in variables from the input class.
	m_Position->MoveMouse(mouseChangeX, mouseChangeY, mouseSensitivity);

	// Get the view point position/rotation.
	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);

	// Reset the cursor to the centre of the screen as we now have mouse movement.
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
	translateMatrix = XMMatrixTranslation(- 3.5f, -10.0f, 0.0f);
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
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixTranslation(0.0f, -10.0f, 0.0f));

	// Render the second model using the light shader.
	m_Model2->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model2->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
    m_Model2->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), 
	m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	if(!result)
	{
		return false;
	}

	//Set rotation, translation and scale for model 3.
	m_D3D->GetWorldMatrix(worldMatrix);
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixScaling(1.0f, 1.0f, 1.0f));
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationY(rotation));
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixTranslation(3.5f, -10.0f, 0.0f));

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
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixScaling(-100.0f, -100.0f, -100.0f));// Revered the scale so that the sphere was turned inside out so the texture renders on the inside.
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixTranslation(posX, posY, posZ));// Sets the current skybox position to the player position so it stays at a constant distance from the player.

	// Render the Skybox using the texture shader. Used the texture shader as the skybox isnt suposed to have shadows, reflections etc.
	m_Skybox->Render(m_D3D->GetDeviceContext());
	m_ShaderManager->RenderTextureShader(m_D3D->GetDeviceContext(), m_Skybox->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
	m_Skybox->GetTexture());

	if (!result)
	{
		return false;
	}

	// Setup the rotation and translation of the Sun.
	m_D3D->GetWorldMatrix(worldMatrix);
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixScaling(1.0f, 1.0f, 1.0f));
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationY(rotation / 16));// Using smaller rotation variable.
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixTranslation(-1500.0f, 0.0f, 0.0f));// Moving sun -1500 units along the x-axis.

	// Render the Sun using the texture shader. As the ligh is coming from the sun it should be completely lit with no shadows.
	m_Sun->Render(m_D3D->GetDeviceContext());
	m_ShaderManager->RenderTextureShader(m_D3D->GetDeviceContext(), m_Sun->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
	m_Sun->GetTexture());

	if (!result)
	{
		return false;
	}

	// Setup the rotation and translation of the Earth.
	m_D3D->GetWorldMatrix(worldMatrix);
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixScaling(0.6f, 0.6f, 0.6f));// Scaling of the Earth (x,y,z).
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationY(rotation / 16));// Rotation around the y-axis.
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationZ(23.5f * radianPerDegree));// The earth is on a 23.5 degree axis, Rotation around z-axis 23.5 degrees converted to radians.
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixTranslation(0.0f, 0.0f, 0.0f));// Translation of the Earth (x,y,z).

	// Render the Earth using the bump map shader.
	m_Earth->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderBumpMapShader(m_D3D->GetDeviceContext(), m_Earth->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
	m_Earth->GetColorTexture(), m_Earth->GetNormalMapTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor());

	if (!result)
	{
		return false;
	}


	// Setup the rotation and translation of the Moon.
	m_D3D->GetWorldMatrix(worldMatrix);
	XMMATRIX moonOrbitRot;
	XMMATRIX moonTran;
	XMMATRIX moonScale;

	// If the moon is Eclipsed then
	if (moonEclipse)
	{
		moonRotationY = 0.0f;// Set moonRotationY equal to 0.0f (Stops it from rotating around the origin).
	}
	else
	{
		moonRotationY += moonOrbitSpeed;// Increase moonRotationY by the moonOrbitSpeed. 
	}

	moonScale = XMMatrixMultiply(worldMatrix, XMMatrixScaling(0.3f, 0.3f, 0.3f));// Scaling of the Moon (x,y,z).
	moonOrbitRot = XMMatrixMultiply(worldMatrix, XMMatrixRotationY(moonRotationY));// Rotation around the y-axis.
	moonTran = XMMatrixMultiply(worldMatrix, XMMatrixTranslation(moonXTranslation, 0.0f, 0.0f));// Positional values (x,y,x).
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationY(rotation / 16));// Slow rotation around the Moon y-axis.

	if (moonEclipse)// If the moon eclipse is enabled the scale and translation is applied without orbit.
	{
		worldMatrix = XMMatrixMultiply(worldMatrix, moonScale * moonTran);
	}
	else // Otherwise the moons scale, translation and rotation is all applied to the worldmatrix causing it to rotate around the origin.
	{
		worldMatrix = XMMatrixMultiply(worldMatrix, moonScale * moonTran * moonOrbitRot);
	}

	// Render Moon using the bump map shader.
	m_Moon->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderBumpMapShader(m_D3D->GetDeviceContext(), m_Moon->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
	m_Moon->GetColorTexture(), m_Moon->GetNormalMapTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor());

	if (!result)
	{
		return false;
	}

	// Setup the rotation and translation of the Shuttle.
	m_D3D->GetWorldMatrix(worldMatrix);
	XMMATRIX shuttleScale;
	XMMATRIX shuttleTrans;
	shuttleScale = XMMatrixMultiply(worldMatrix, XMMatrixScaling(10.0f, 10.0f, 10.0f));// Scaling of the shuttle (x,y,z).
	shuttleTrans = XMMatrixMultiply(worldMatrix, XMMatrixTranslation(shuttleDistance, 0.0f, 0.0f));// Positional values for the shuttle (x,y,x).
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationY(shuttleRotation));// Gets the rotation required to face the Moon or Earth. Controlled by shuttleController.
	worldMatrix = XMMatrixMultiply(worldMatrix, shuttleScale * shuttleTrans * moonOrbitRot);// Add the matrixes together by multiplication and then add them to the shuttles world matrix by again multiplication.
	// Used the moonrotation as my rotation so the shuttle is always facing and on the path to the moon whatever the translation and scale is.

	// Render Shuttle using the bump map shader.
	m_Shuttle->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Shuttle->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
	m_Shuttle->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
	m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	if (!result)
	{
		return false;
	}

	// Setup the rotation and translation of the Satalite.
	m_D3D->GetWorldMatrix(worldMatrix);
	XMMATRIX sateliteScale;
	XMMATRIX sateliteRot;
	XMMATRIX sateliteRotX;
	XMMATRIX sateliteTrans;
	sateliteScale = XMMatrixMultiply(worldMatrix, XMMatrixScaling(5.0f, 5.0f, 5.0f));// Scaling of the satalite (x,y,z).
	sateliteRot = XMMatrixMultiply(worldMatrix, XMMatrixRotationY(rotation / 8));// Rotation around the y-axis.
	sateliteTrans = XMMatrixMultiply(worldMatrix, XMMatrixTranslation(0.0f, 0.0f, -45.0f));// Positional values (x,y,x).
	worldMatrix = XMMatrixMultiply(worldMatrix, sateliteScale * sateliteTrans * sateliteRot);// Applying all matrixes to the worldMatrix together.

	// Render the Satalite using the bump map shader.
	m_Satelite->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Satelite->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
	m_Satelite->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
	m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	if (!result)
	{
		return false;
	}

	// Setup the rotation and translation of the UFO.
	m_D3D->GetWorldMatrix(worldMatrix);
	XMMATRIX ufoScale;
	XMMATRIX ufoRotY;
	XMMATRIX ufoRotZ;
	XMMATRIX ufoTrans;
	ufoScale = XMMatrixMultiply(worldMatrix, XMMatrixScaling(0.005f, 0.005f, 0.005f));// Scaling of the UFO (x,y,z).
	ufoRotY = XMMatrixMultiply(worldMatrix, XMMatrixRotationY(rotation / 8));// Rotation around y-axis.
	ufoRotZ = XMMatrixMultiply(worldMatrix, XMMatrixRotationZ(rotation / 4));// Rotation around z-axis.
	ufoTrans = XMMatrixMultiply(worldMatrix, XMMatrixTranslation(0.0f, 45.0f, 0.0f));// Positional values (x,y,x).
	worldMatrix = XMMatrixMultiply(worldMatrix, ufoScale * ufoTrans * (ufoRotY * ufoRotZ));// Applying all matrixes to the worldMatrix together.

	// Render the UFO using the bump map shader.
	m_UFO->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_UFO->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
	m_UFO->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
	m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	if (!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();


	return true;
}

void GraphicsClass::SetCameraPosition(float posX, float posY, float posZ, float rotX, float rotY, float rotZ)
{
	// Reset the camera to the original position...
	m_Position->SetPosition(posX, posY, posZ);
	m_Position->SetRotation(rotX, rotY, rotZ);
}


