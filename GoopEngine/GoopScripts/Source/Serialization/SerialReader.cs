﻿using System;
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

    static public PlayerStatsInfo LoadPlayerState(string fileName)
    {
      PlayerStatsInfo ret = new PlayerStatsInfo();
      using (StreamReader sr = new StreamReader(fileName))
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
        } while ((line = sr.ReadLine()) != null) ;          
      }

      return ret;
    }

    static public EnemyStatsInfo LoadEnemy(string fileName)
    {
      EnemyStatsInfo ret = new EnemyStatsInfo();
      using (StreamReader sr = new StreamReader(fileName))
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

    static public void SavePlayerState(ref Gameplay.Stats stats, int currentLevel, string fileName)
    {
      using (StreamWriter sw = new StreamWriter(fileName))
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

    static public Dictionary<Gameplay.CharacterType, Dictionary<CardBase.CardID, string>> LoadAnimationMappings(string file)
    {
      var ret = new Dictionary<Gameplay.CharacterType, Dictionary<CardBase.CardID, string>>();
      using (StreamReader sr = new StreamReader(file))
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

    public static void AddCardsToDeck(List<CardBase.CardID> cards, string file)
    {
      string[] lines = File.ReadAllLines(file);
      int targetIndex = Array.FindLastIndex(lines, line => !string.IsNullOrEmpty(line));
      if (targetIndex == -1)
      {
        targetIndex = lines.Length - 1;
      }

      using (StreamWriter writer = new StreamWriter(file))
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

    public static void IncrementLevel(string file)
    {
      string[] lines = File.ReadAllLines(file);
      int level;
      if (lines.Length == 0 || !int.TryParse(lines[1], out level))
      {
#if (DEBUG)
        Utils.SendString("Unable to increment level in file: " + file);
#endif
        return;
      }
      lines[1] = (level + 1).ToString();

      File.WriteAllLines(file, lines);
    }

    static string GetNextNonCommentLine(StreamReader sr)
    {
      string line;
      while ((line = sr.ReadLine()) != null && (line.StartsWith(COMMENT_SYMBOL) || string.IsNullOrWhiteSpace(line))) ;

      return line;
    }
  }
}
