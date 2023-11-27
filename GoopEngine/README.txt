- Our sprites are to be in this format: 32bit RGBA (images with 24 bit depth are not supported!)
- Assigning script "Root Node" to a composite node will crash the program.
- Ending turn while the turn is progressing (Cards switching out and card drawing) leads to undefined behaviour.

In the gameplay main menu scene. Only the "New Game" & "Quit Game" button works.
In the game scene. You can queue and unqueue up to 3 cards by clicking the card asset and then press end turn to resolve the turn.
The turn ends once all cards are resolved and Leah goes back to her idle animation.
The cards do not do anything as of now. (Ending the turn again while the turn is still resolving leads to undefined behaviour) 
During gameplay, unfocusing the window or pressing escape will bring up the pause pop up. 
In the pause pop up, the "How to Play", "Resume" and "Quit" button along with the 'X' button works.
Pressing "1" will bring up the Victory screen. The "Leave" button works and returns to the main menu.
Pressing "2" will bring up the Defeat screen. The "Leave" button works and returns to the main menu.