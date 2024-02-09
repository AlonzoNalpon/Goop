- Our sprites are to be in this format: 32bit RGBA (images with 24 bit depth are not supported!)
- Assigning script "Root Node" to a composite node will crash the program.
- DUring the resolve phase, if cards are being selected and deselected, it leads to undefined behaviour.
- Ending the turn again while the turn is still resolving leads to undefined behaviour.
- While resolving, if you press "1" or "2" and leave, then go back into the game, the resolve phase will start
from the beginning and play even if you did not select any cards or end the turn after returning to the game.

In the gameplay main menu scene. Only the "New Game", "Options", "How to play", "Credits" & "Quit Game" button works.

In the cutscenes:
By pressing Left Mouse Button, you can move to the next scene.

In the game scene:
You can queue and unqueue up to 3 cards by clicking the cards in the player's hand and then press end turn to resolve the turn.
The cards will be resolved, affecting the player and enemy's health.
Each card will have their own animations, i.e. Leah's Beam card will play Leah's Beam animation.
Two adjacent selected cards will form a card combo.
The combos will be applied after the resolution of the 2 cards.
The combo effect will be displayed for a brief moment as text beside the queued cards.
An icon for buffs and debuffs applied will appear above the character's respective health bar.
Hovering over the icons will display text to show the type of buff.
The turn ends once all cards and combos are resolved.

During gameplay, unfocusing the window or pressing escape will bring up the pause menu. 
In the pause menu, only the "How to Play", "Resume", "Surrender", "X", "Yes" and "No" button works.

Controls:
On the editor/engine, Pressing "G" when viewport is focused will hide the editor.
"F11" on both gameplay and editor can toggle fullscreen modes

Cheats:
Pressing "U", in the game scene, will reduce the enemy's health by 1 bar.

known bugs:

Team Roster:
Wong Chin Kit Bryan - Tech Lead, Product Manager
Alonzo Maurice Nalpon - Graphics
Han Qin Ding - Input, Scripting
Cheng En Lau - Serialization
Joel Loh Kong Iann - Assets Manager, Object Factory
Chrissylia Phua - Design Lead, Audio Lead
Jonathan Lee Jun Wee - Art Lead
Dana Lee Syu Ling - Artist