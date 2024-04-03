/*!*********************************************************************
\file   SerialReader.cs
\author chengen.lau\@digipen.edu
\date   15-March-2024
\brief  This class handles the loading and saving of game level data.
        It primarily reads from raw text files under a specified format.
        Improvements to be made are to generate a default save file if
        it doesn't exist.
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Drawing.Drawing2D;
using GoopScripts.Cards;
using System.Diagnostics;
using GoopScripts.Gameplay;
using System.Security.Cryptography.X509Certificates;
using GoopScripts.Mono;

namespace GoopScripts.Serialization
{
  static public class SerialReader
  {
    static readonly string COMMENT_SYMBOL = "#";
    static readonly string SAVE_DIRECTORY = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments) + "\\Goop\\DungeonPlunder\\";
    static readonly string PLAYER_STATS_FILE = SAVE_DIRECTORY + "PlayerStats.sav";

    /*!*********************************************************************
    \brief
      Reads from the given file and returns the data in the form of a
      PlayerStatsInfo object. The file should be formatted in the following
      way:
        <level_number>
        <health, max_health>
        <deck_list> (in the format of <card, quantity> per line)
    \param fileName
      The file to load
    ************************************************************************/
    static public PlayerStatsInfo LoadPlayerState(bool isTutorial = false)
    {
      CheckSaveDirectory(true);

      string file = isTutorial ? "./Assets/GameData/TutorialStats.sav" : PLAYER_STATS_FILE;
      PlayerStatsInfo ret = new PlayerStatsInfo();
      using (StreamReader sr = new StreamReader(file))
      {
        string line = GetNextNonCommentLine(sr);
        if (!int.TryParse(line, out ret.levelToLoad))
        {
#if (DEBUG)
          Console.WriteLine("Invalid level read:\n" + line);
#endif
        }

        line = GetNextNonCommentLine(sr);
        string[] healthData = line.Split(',');
        if (!int.TryParse(healthData[0], out ret.health) || !int.TryParse(healthData[1], out ret.maxHealth))
        {
#if (DEBUG)
          Console.WriteLine("Invalid data read for health:\n" + line);
#endif
        }

        line = GetNextNonCommentLine(sr);
        ret.deckList = new List<Tuple<CardBase.CardID, uint>>();
        do
        {
          string[] cardData = line.Split(',');
          CardBase.CardID card;
          uint amount;
          if (!Enum.TryParse(cardData[0], out card) || !uint.TryParse(cardData[1], out amount))
          {
#if (DEBUG)
            Console.WriteLine("Invalid card entry read for deck:\n" + line);
#endif      
            continue;
          }
          ret.deckList.Add(new Tuple<CardBase.CardID, uint>(card, amount));
        } while ((line = sr.ReadLine()) != null);
      }

      return ret;
    }

    /*!*********************************************************************
    \brief
      Reads from the given file and returns the data in the form of a
      EnemyStatsInfo object. The file should be formatted in the following
      way:
        <enemy_type>
        <prefab_name>
        <potrait_sprite>
        <background_prefab>
        <health, max_health>
        <deck_list> (in the format of <card, quantity> per line)
    \param fileName
      The file to load
    ************************************************************************/
    static public EnemyStatsInfo LoadEnemy(string fileName)
    {
      EnemyStatsInfo ret = new EnemyStatsInfo();
      using (StreamReader sr = new StreamReader(File.OpenRead(fileName)))
      {
        string line = GetNextNonCommentLine(sr);
        if (!Enum.TryParse(line, out ret.characterType))
        {
#if (DEBUG)
          Console.WriteLine("Invalid character type read:\n" + line);
#endif
        }

        ret.prefab = GetNextNonCommentLine(sr);
        ret.portrait = GetNextNonCommentLine(sr);
        ret.background = GetNextNonCommentLine(sr);

        line = GetNextNonCommentLine(sr);
        string[] healthData = line.Split(',');
        if (!int.TryParse(healthData[0], out ret.health) || !int.TryParse(healthData[1], out ret.maxHealth))
        {
#if (DEBUG)
          Console.WriteLine("Invalid data read for health:\n" + line);
#endif
        }

        line = GetNextNonCommentLine(sr);
        ret.deckList = new List<Tuple<CardBase.CardID, uint>>();
        do
        {
          string[] cardData = line.Split(',');
          CardBase.CardID card;
          uint amount;
          if (!Enum.TryParse(cardData[0], out card) || !uint.TryParse(cardData[1], out amount))
          {
#if (DEBUG)
            Console.WriteLine("Invalid card entry read for deck\n" + line);
#endif      
            continue;
          }
          ret.deckList.Add(new Tuple<CardBase.CardID, uint>(card, amount));
        } while ((line = sr.ReadLine()) != null);
      }

      return ret;
    }

    /*!*********************************************************************
    \brief
      Saves the player's current level, health and deck into a
      PlayerStatsInfo object. The file will be formatted in the following
      way:
        <level_number>
        <health, max_health>
        <deck_list> (in the format of <card, quantity> per line)
    \param stats
      The player's stats object
    \param currentLevel
      The current level
    \param fileName
      The file to save to
    ************************************************************************/
    static public void SavePlayerState(ref Gameplay.Stats stats, int currentLevel)
    {
      CheckSaveDirectory();


      using (StreamWriter sw = new StreamWriter(PLAYER_STATS_FILE))
      {
        // serialize level
        sw.WriteLine(COMMENT_SYMBOL + " Level to load");
        sw.WriteLine(currentLevel);

        // serialize health
        sw.WriteLine();
        sw.WriteLine(COMMENT_SYMBOL + " Health, MaxHealth");
        sw.WriteLine(stats.m_healthBar.m_health + ", " + stats.m_healthBar.m_maxHealth);

        // serialize deck list
        sw.WriteLine();
        sw.WriteLine(COMMENT_SYMBOL + " Deck List");
        foreach (var elem in stats.m_deckMngr.m_deck.GetDeckListWithCount())
        {
          sw.WriteLine(elem.Key.ToString() + ", " + elem.Value.ToString());
        }
      }
    }

    /*!*********************************************************************
    \brief
      Reads from the file to load the mappings of each card to each
      character's animation spritesheet. The data is returned in the form
      of a dictionary where the key is the CharacterType enum and the
      value is a dictionary of cardID to animation name 
    \param file
      The file to read from
    ************************************************************************/
    static public Dictionary<Gameplay.CharacterType, Dictionary<CardBase.CardID, string>> LoadAnimationMappings(string file)
    {
      var ret = new Dictionary<Gameplay.CharacterType, Dictionary<CardBase.CardID, string>>();
      using (StreamReader sr = new StreamReader(File.OpenRead(file)))
      {
        string line = GetNextNonCommentLine(sr);
        while (line != null)
        {
          Gameplay.CharacterType charType;
          if (!Enum.TryParse(line, out charType))
          {
#if (DEBUG)
            Console.WriteLine("Invalid character type read:\n" + line);
#endif  
          }

          var mappings = new Dictionary<CardBase.CardID, string>();
          while ((line = GetNextNonCommentLine(sr)) != null)
          {
            string[] cardToAnim = line.Split(',');
            CardBase.CardID card;
            string anim;

            // read until unable to parse,
            // which means a new enemy's animations
            if (!Enum.TryParse(cardToAnim[0], out card))
            {
              break;
            }
            anim = cardToAnim[1].Trim();

            mappings.Add(card, anim);
          }
          ret.Add(charType, mappings);
        }
      }

      return ret;
    }

    /*!*********************************************************************
    \brief
      Adds cards directly into the player save file. The data is appended
      at the end of the file at the last line that is not a whitespace
    \param cards
      The List of cards to add
    \param file
      The file to save to
    ************************************************************************/
    public static void AddCardsToDeck(List<CardBase.CardID> cards)
    {
      CheckSaveDirectory();

      string[] lines = File.ReadAllLines(PLAYER_STATS_FILE);
      int targetIndex = Array.FindLastIndex(lines, line => !string.IsNullOrEmpty(line));
      if (targetIndex == -1)
      {
        targetIndex = lines.Length - 1;
      }

      using (StreamWriter writer = new StreamWriter(File.OpenRead(PLAYER_STATS_FILE)))
      {
        for (int i = 0; i <= targetIndex; ++i)
        {
          writer.WriteLine(lines[i]);
        }

        foreach (CardBase.CardID card in cards)
        {
          writer.WriteLine(card.ToString() + ", 1");
        }
      }
    }

    /*!*********************************************************************
    \brief
      Increments the level number inside the player's save file
    \param file
      The file to modify
    ************************************************************************/
    public static void IncrementLevel()
    {
      CheckSaveDirectory();

      string[] lines = File.ReadAllLines(PLAYER_STATS_FILE);
      int level;
      if (lines.Length == 0 || !int.TryParse(lines[1], out level))
      {
#if (DEBUG)
        Utils.SendString("Unable to increment level in file: " + PLAYER_STATS_FILE);
#endif
        return;
      }
      lines[1] = (level + 1).ToString();
      Console.WriteLine("Level is incremented from " + level + " to " + (level + 1));

      File.WriteAllLines(PLAYER_STATS_FILE, lines);
    }

    /*!*********************************************************************
    \brief
      Reads from the file to get the next line that is not a comment
    \param sr
      The StreamReader object
    ************************************************************************/
    static string GetNextNonCommentLine(StreamReader sr)
    {
      string line;
      while ((line = sr.ReadLine()) != null && (line.StartsWith(COMMENT_SYMBOL) || string.IsNullOrWhiteSpace(line))) ;

      return line;
    }

    /*!*********************************************************************
    \brief
      Checks if SAVE_DIRECTORY exists. If it is missing, it will be
      generated.
    \param generateSaveFile
      If true, checks for and generates a default save file if it doesn't
      exist
   ************************************************************************/
    static void CheckSaveDirectory(bool generateSaveFile = false)
    {
      if (!Directory.Exists(SAVE_DIRECTORY))
      {
        Directory.CreateDirectory(SAVE_DIRECTORY);
      }

      if (generateSaveFile && !File.Exists(PLAYER_STATS_FILE))
      {
        GenerateDefaultSave();
      }
    }

    static public void GenerateDefaultSave()
    {
      string saveStr = @"# Level to load
0

# Health, MaxHealth
10, 10

# Deck List
LEAH_BEAM, 4
LEAH_STRIKE, 4
LEAH_SHIELD, 4
SPECIAL_FLASHBANG, 1";

      if (!Directory.Exists(SAVE_DIRECTORY))
      {
        Directory.CreateDirectory(SAVE_DIRECTORY);
      }
      File.WriteAllText(PLAYER_STATS_FILE, saveStr);
    }
  }
}
