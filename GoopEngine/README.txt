- Our sprites are to be in this format: 32bit RGBA (images with 24 bit depth are not supported!)
- Assigning script "Root Node" to a composite node will crash the program.
- DUring the resolve phase, if cards are being selected and deselected, it leads to undefined behaviour.
- Ending the turn again while the turn is still resolving leads to undefined behaviour.
- While resolving, if you press "1" or "2" and leave, then go back into the game, the resolve phase will start
from the beginning and play even if you did not select any cards or end the turn after returning to the game.

In the gameplay main menu scene. Only the "New Game" & "Quit Game" button works.
In the game scene. You can queue and unqueue up to 3 cards by clicking the card asset and then press end turn to resolve the turn.
The turn ends once all cards are resolved and Leah goes back to her idle animation.
The cards do not do anything as of now. 
During gameplay, unfocusing the window or pressing escape will bring up the pause menu. 
In the pause menu, the "How to Play", "Resume", "Quit", "X", "Yes" and "No" button works.
Pressing "1", in the game scene, will bring up the Victory screen. The "Leave" button works and returns to the main menu.
Pressing "2", in the game scene, will bring up the Defeat screen. The "Leave" button works and returns to the main menu.