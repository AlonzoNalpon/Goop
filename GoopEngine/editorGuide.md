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
Directory holds all folders and sub folders within Asset Directory.
View displays files within each folder selected from Directory. Images are printed for .png files.

[ Node Editor ]
Allows user to edit enemy's behaviour tree. Any changes made will be displayed in the game at runtime, if the tree is valid (will explain more about it bellow)
There are 3 types of nodes available, Root Node, Composite Node and Leaf Node (Each of them are displayed with a different colored heading)
There are 2 types of pins, Input Pin (Left Pin) and the Output Pin (Right pin). 
Currently, you can navigate around the node editor by holding the middle mouse button and move your mouse around

Users are able to perform these actions:
1. Reposition the nodes
2. Link/Unlink nodes through the pins to create a parent/child relationship
   - A link is made by connecting an output pin to an input pin. The outpin represents the parent in the relationship while the inputpin represents the child in the relationship. 
   - To Form a link click on thepin and drag it to the other pin. Same thing if you want to remove the link
   - Root Node only has output pin as in can only be a parent, Composite Node has both output and input as it can be a parent and a child at the same time and the Leaf Node only has an input pin has it can only be a 
     child
3. Attach Scripts to a node
   - You can attach 1 script to each node. Each node has a box that shows the name of the script attached to node. when you click on the box, it will show a list of all the scripts you can attach to the node
   - In order for the tree to run as a behaviour tree, you will Root Node should only have the "RootNode" script and your Composite Nodes should only have the "SelectorNode" or "SeqeunceNode" scripts. You can attach 
      the rest of the other scripts to the leaf nodes
   - If you try to attach other scripts to it, the tree will still run, but not as a behaviour tree
4. Add Node
   - If you did not select any nodes and click the right mouse button, you will be able to see 3 options (Add root Node, Add composite Node, Add leaf node)
   - By selecting any of these 3 options, it will spawn a new empty node with no script. If you have any nodes with no scripts, it will not update the behaviour tree in game. So you should attach a script to it 
     after create the new node ( Do please attach the right script)
5. Edit Node
   - If you select a node and click the right mouse button, you will be able to see the "delete node" option and any 2 of these ("Change to root Node, Change to composite node, Change to leaf node") options based on 
     the type of node you select. e.g. (If you select the root node, you will only see ("delete node, change to composite node, change to leaf node"))
   - If you choose to change the node type, it will remove any links the node had, but the script remains unchanged.
   - If you choose to delete the node, it will remove the node and any links it had


[ Prefab Editor ]