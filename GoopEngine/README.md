
[ Solution ]
On startup, either select debug or release mode
For target, set to x64 instead of "Any CPU"

[ IMGUI ]
G -> open imgui menu
Spawn objects in imgui
Spawn prefab in imgui
clone entity in imgui

*error logger can be seen by trying to spawn more entities than max entity set in config

[ Physics ]
press space -> step update physics once
hold space -> keep updating physics

[Player Control MonoScript]
Player is the interpolating sprite
W,A,S,D -> Move player 
E,R -> Rotate player clockwise/counter clockwise
Scroll Mouse-> Increase/Decrease the size of the player

*For ease of use, player controller script will ignore step update

[ Multiple Animations ]
K -> change the player animation

[ Events ]
InputManager polls for key/mouse triggers and holds
When detected, it dispatches the event together with the key code
The PlayerController and DraggableObject Systems who are subscribed to the event
receive the event and handles it through their HandleEvent() function
Running in Debug mode will show dispatching and handling printed on key/mouse press
