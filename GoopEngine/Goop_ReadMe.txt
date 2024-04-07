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

Notes:
  - The game stores the save file under "Documents\Goop\DungeonPlunder\" directory
  - The "Load Game" option brings you back to the beginning of the last level you stopped at
    You can use this option to continue a run from the level you lost
    This will however skip the tutorial if used at the start of the game so we recommend
    the "New Game" button for a normal playthrough.

Pausing:
  - Press ESC key to pause
    Pausing the game pauses the game scene.

Controls:
  - On the editor/engine, Pressing "G" when viewport is focused will hide the editor.
  - "F11" on both gameplay and editor can toggle fullscreen modes
  - "P" in-game will trigger the FPS counter on and off.

Cheats (in game scene):
  - Pressing "U" will reduce the enemy's health by 1.
  - Pressing "I" will heal the player's health by 1.
  - Pressing "O" will draw an additional card
    (note that max hand size is 5 and drawing past this will cause you to lose the card for the rest of this battle)

Known bugs:
 - During the tutorial, the card hover effect may remain on the screen when proceeding to the next step. This is
   due to the overlay blocking it from detecting the cursor hover.
 - Occasionally, hovering on a card while it moves may cause the hover effect to be placed wrongly. It will revert
   back to normal upon hovering over another card.
 - When moving the cursor really fast, the cards sometimes do not display the hover effect.
 - Occasionally, cards, buff icons may render with missing elements (e.g. text). 

**These are all visual glitches and have no effect on the gameplay. The cards can still be queued/unqueued and game can go on as per normal.

Team Roster:
Wong Chin Kit Bryan - Tech Lead, Product Manager
Alonzo Maurice Nalpon - Graphics
Han Qin Ding - Input, Scripting
Cheng En Lau - Serialization
Joel Loh Kong Iann - Assets Manager, Object Factory
Chrissylia Phua - Design Lead, Audio Lead
Jonathan Lee Jun Wee - Art Lead
Dana Lee Syu Ling - Artist