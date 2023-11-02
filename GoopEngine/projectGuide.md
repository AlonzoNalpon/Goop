
[ Solution ]
.NET Framework 4.72 MUST BE INSTALLED to run the project (for scripting)
On startup, either select debug or release mode
For target, set to x64 instead of "Any CPU"

[ Scenes ]
Program starts at a sample scene
Press right arrow to switch to testing scene
Press left to switch back

[Player Control MonoScript]
Player is the interpolating sprite
W,A,S,D -> Move player 
E,R -> Rotate player clockwise/counter clockwise
Scroll Mouse-> Increase/Decrease the size of the player

[ Multiple Animations ]
Animations do not begin playing until simulation is started
K -> change the player animation

[ SCENE LOADING/SAVING ]
From Toolbar, click on "File" > "Open" > Select any ".scn" file, it will load it to the
state it was last saved in.
"File" > "Save" to save the current state of the scene to the file that it loaded from
"File" > "New Scene" and enter filename + ".scn" to create a new scene and load it

*Note: The file explorer may not open to the solution directory sometimes.
       In that case, navigate to GoopEngine/Editor/Assets/Scenes

[ EVENTS SYSTEM (SUBSCRIBER-LISTENER) ]
InputManager polls for key/mouse triggers and holds ("DispatchInputEvents" from InputManager.cpp)
When detected, it dispatches the event together with the key code
The PlayerController (K,E,Q,R) and DraggableObject Systems (LEFT MOUSE HOLD) who are subscribed to the event
receive the event and handles it through their HandleEvent() function
*To display output for events, you can uncomment the #define EVENT_DEBUG flag in file pch.h
