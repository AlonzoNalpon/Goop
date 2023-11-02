***All these are tabs and can be docked/undocked***

[ ToolBar ]
File
  - New scene
  - Save current scene
  - Load scene (Opens File explorer)
View
  - Currently only has toggle for performance graph
Options
  - Currently empty

[ Viewport ]
Resizable and truncates rendered texture
Able to pick objects (but not drag to transform)

[ Inspector ]
Not RTTR integrated yet
Components can be added via the Add Component button at the end of the component list
Components (except transform) can be removed by right clicking the component name header
Able to view the following components:
Transform (Every entity MUST have a transform)
Sprite
Velocity
Tween
Draggable
Text

[ Performance Graph ]
All recorded times are shown in milliseconds
System time percentage may show nan due to number being too low to calculate

[ Console ]
Info, Errors and Warnings are logged into this
You can change the minimum shown log level with the drop down

[ Hierachy ]
Drag and drop entities onto each other to parent/unparent them
Child entities should follow parent
You can right click entities to duplicate/delete them
You can right click the anywhere in the hierchy tab to create a new entitiy

[ Scenes ]
Program starts at a sample scene
Press right arrow key to switch to testing scene
Press left arrow key to switch back

[ Audio ]
Background music is hardcoded to begin playing when the test scene is loaded
There exist a temporary audio tab with buttons to play sounds
Q,E,R also plays some sound effects

[ Extras ]
You can edit the spatial partitoning grid here (5x5 is the most efficient from initial testing)
Button to duplicate 500 of the selected entity
Depreciated but still working button to spawn a worm prefab

[ Asset Browser ]

[ Node Editor ]

[ Prefab Editor ]