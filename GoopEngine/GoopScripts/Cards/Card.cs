using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Cards
{
	internal class Card
	{
		// These enums of unqiue card IDs
		// CardIDs are encoded in the following bit format
		// <Entity Type><Card Category><Actual ID>
		// Basic = 0, Dawnson = 1, Player = 2
		// Attack = 0, Block = 1, Special = 2
		// Unique card ID in the category starting from 0
		// Eg. Basic enemy, special card, first and only card
		// 0x020
		// Current system supportd up to 16 cards in a category
		public enum CardID
		{
			// Basic enemy cards
			BASIC_LUNGE_ATTACK = 0x000,
			BASIC_NORMAL_ATTACK = 0x001,

			BASIC_NORMAL_BLOCK = 0x010,

			BASIC_BUFF_2X_ATTACK = 0x020,

			// Player cards
			PLAYER_BEAM_ATTACK = 0x100,
			PLAYER_EMPOWERED_ATTACK = 0x101,
			PLAYER_EMPOWERED_SWING = 0x102,

			PLAYER_NORMAL_BLOCK = 0x110,

			PLAYER_DEBUFF_FLASH_BANG = 0x120,
			PLAYER_BUFF_SMOKESCREEN = 0x121,
			PLAYER_BUFF_RAGE = 0x122,
			PLAYER_EMPOWERED_BEAM_ATTACK = 0x123,

			// Dawson enemy cards
			DAWSON_BEAM_ATTACK = 0x200,
			DAWSON_EMPOWERED_ATTACK = 0x201,

			DAWSON_NORMAL_BLOCK = 0x210,

			DAWSON_DEBUFF_CHARGE_ATTACK = 0x220,
			DAWSON_DEBUFF_SPEED_UP_QUEUE = 0x221,
		}
	}
}
