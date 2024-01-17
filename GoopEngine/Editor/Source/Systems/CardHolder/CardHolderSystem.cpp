#include <pch.h>
#include <Systems/CardHolder/CardHolderSystem.h>
#include <Component/Sprite.h>
#include <Component/Card.h>
#include <GameDef.h>
namespace GE::Systems
{
  void CardHolderSystem::Update()
  {
    for (GE::ECS::Entity entity : GetUpdatableEntities())
    {
      Component::CardHolder* holder{ m_ecs->GetComponent<Component::CardHolder>(entity) };
      
      if (holder->dataType == Component::CardHolder::DataType::NONE)
        continue; // skip if there's no data to read from

      // Now find the c# script that contains the data
      Component::Scripts* allScripts{ m_ecs->GetComponent<Component::Scripts>(holder->dataEntity) };
      // Assumption is that it is a vector of unsigned values
      
      constexpr const char* scriptName{ "Stats" }; // This is hardcoded because the logic requires this script's structure
      // Find the right script by name
      {
        GE::MONO::ScriptInstance* statsScript{ allScripts->Get(scriptName) };
        statsScript->GetAllUpdatedFields();

        // In this order: m_cardQueue -> m_deck -> m_hand
        //rttr::variant& cardQueueInst{ statsScript->m_scriptFieldInstList[0]};
        //rttr::variant& deckInst     { statsScript->m_scriptFieldInstList[1]};
        //rttr::variant& handInst     { statsScript->m_scriptFieldInstList[2]};
        rttr::variant& targetInst{ statsScript->m_scriptFieldInstList[holder->dataType] };

        //std::vector<unsigned> const& queueData{ cardQueueInst.get_value<GE::MONO::ScriptFieldInstance<std::vector<unsigned>>>().m_data };
        //std::vector<unsigned> const& deckData { deckInst.get_value<GE::MONO::ScriptFieldInstance<std::vector<unsigned>>>().m_data };
        //std::vector<unsigned> const& handData { handInst.get_value<GE::MONO::ScriptFieldInstance<std::vector<unsigned>>>().m_data };

        std::vector<unsigned> const& targetData{ targetInst.get_value<GE::MONO::ScriptFieldInstance<std::vector<unsigned>>>().m_data };
        targetData;
        // Now edit the cards in the elements for holder
        auto dataIt{ targetData.begin() };
        auto gameCardIt{ holder->elements.begin() };
        auto const& ge = Graphics::GraphicsEngine::GetInstance();
        while (dataIt != targetData.end() && gameCardIt != holder->elements.end())
        {
          auto* card = m_ecs->GetComponent<Component::Card>(gameCardIt->elemEntity);
          auto* sprite = m_ecs->GetComponent<Component::Sprite>(gameCardIt->elemEntity);

          // Update the card id
          card->cardID = static_cast<Component::Card::CardID>(*dataIt);

          sprite->m_spriteName = CardSpriteNames[card->cardID];
          sprite->m_spriteData.texture = ge.textureManager.GetTextureID(sprite->m_spriteName);

          ++dataIt;
          ++gameCardIt;
        }
      }
    }
  }
}
