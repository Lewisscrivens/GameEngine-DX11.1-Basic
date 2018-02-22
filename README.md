# GameEngine-DX11

A DirectX 11 Game Engine created for my assignment in Game Engine Architecture
at The University of Huddersfield.

----------------------------------------------------------------------------------

Features:

- Omnidirectional movement within the editor.
- Movement speed increase method.
- Skybox.
- Example scene.

----------------------------------------------------------------------------------

Example scene:

The scene contains the earth and the moon orbiting the earth along with a satelite and alien UFO. The is also a space shuttle that will go between the earth and the moon. The sun is shown in the distance as I only had directional light working.

----------------------------------------------------------------------------------

Controls:

Fullscreen: F11 [NOT WORKING CURRENTLY]

Camera Up: Mouse y-axis

Camera Right: Mouse x-axis

Forward: W
Back: S
Left: A
Right: D
Up: Space Bar
Down: Left Ctrl
Increase Camera Speed: Left Shift
Moon Orbit Speed:
Increase: NUM_PLUS
Decrease: NUM_MINUS
Reset: NUM_0
Moon Eclipse:
Enable: 0
Disable: 9
Reset Camera Location: R
Set Camera Start Location: H
Launch Shuttle: M
Increase Shuttle Speed: PgUp
Decrease Shuttle Speed: PgDn

----------------------------------------------------------------------------------

Refferences are shown bellow.

3D Models:

The 3D models used was converted into co-ordinates in .txt files so that the model class could construct them in the engine.

 - Satellite       - https://www.turbosquid.com/FullPreview/Index.cfm/ID/337131
 - Space Shuttle   - https://www.turbosquid.com/FullPreview/Index.cfm/ID/337135
 - Sphere          - Made in 3DS Max.


Textures provided by Huddersfield University:

Any textures in the engine must be .DDS as it is currently the only supported texture files.

 - Metal.DDS
 - Marble.DDS
 - Stone.DDS
 
The remaining textures was all taken from the following website link:

 - https://www.solarsystemscope.com/textures
