#include <pch.h>
#include "ScriptInstance.h"
#include "ScriptManager.h"
#include "CSharpStructs.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
#include "mono/metadata/tabledefs.h"
#include "mono/metadata/mono-debug.h"
#include "mono/metadata/threads.h"
/*!*********************************************************************
\file   ScriptInstance.cpp
\author han.q\@digipen.edu
\date   28 September 2023
\brief
  Script Class to contain the data of a c# class.
  It will store the pointer to the MonoObject and its 4 main methods


Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/

using namespace GE;
using namespace MONO;


void ScriptField::SetField(std::string const& className)
{
  GE::MONO::ScriptManager* sm = &GE::MONO::ScriptManager::GetInstance();
  *this = sm->GetScriptField(className, m_fieldName);
}

ScriptInstance::ScriptInstance(const std::string& scriptName, std::vector<void*>& arg) : m_scriptName{ scriptName }
{

  GE::MONO::ScriptManager* sm = &GE::MONO::ScriptManager::GetInstance();
  m_scriptClass = sm->GetScriptClass(scriptName);
  m_classInst = sm->InstantiateClass(scriptName.c_str(), arg);
  m_onUpdateMethod = mono_class_get_method_from_name(m_scriptClass,"OnUpdate", 1);
  m_gcHandle = mono_gchandle_new(m_classInst, true);
  m_entityID = GE::ECS::INVALID_ID;
  m_onCreateMethod = mono_class_get_method_from_name(m_scriptClass, "OnCreate", 0);
}


ScriptInstance::ScriptInstance(const std::string& scriptName, GE::ECS::Entity  entityID) : m_scriptName{ scriptName }, m_entityID{entityID}
{  
  std::vector<void*> params = { &entityID };
  GE::MONO::ScriptManager* sm = &GE::MONO::ScriptManager::GetInstance();
  m_scriptClass = sm->GetScriptClass(scriptName);
  m_classInst = sm->InstantiateClass(scriptName.c_str(), params);
  m_onUpdateMethod = mono_class_get_method_from_name(m_scriptClass, "OnUpdate", 1);
  m_gcHandle = mono_gchandle_new(m_classInst, true);
  GetFields();
  m_onCreateMethod = mono_class_get_method_from_name(m_scriptClass, "OnCreate", 0);
}


void ScriptInstance::InvokeOnUpdate(double dt)
{
  if (m_onUpdateMethod)
  {
    std::vector<void*> params = { &dt };
    mono_runtime_invoke(m_onUpdateMethod, mono_gchandle_get_target(m_gcHandle), params.data(), nullptr);
  }
}

void ScriptInstance::InvokeOnUpdate(GE::ECS::Entity m_entityId, double dt)
{
  if (m_onUpdateMethod)
  {
    std::vector<void*> params = { &m_entityId, &dt };
    mono_runtime_invoke(m_onUpdateMethod, mono_gchandle_get_target(m_gcHandle), params.data(), nullptr);
    //mono_runtime_invoke(m_onUpdateMethod, m_classInst, nullptr, nullptr);
  }
}


void ScriptInstance::InvokeOnCreate()
{
  if (m_onCreateMethod)
  {
    mono_runtime_invoke(m_onCreateMethod, mono_gchandle_get_target(m_gcHandle),nullptr, nullptr);
  }
} 

void ScriptInstance::GetFields()
{
  GE::MONO::ScriptManager* sm = &GE::MONO::ScriptManager::GetInstance();
  ScriptClassInfo sci = sm->GetScriptClassInfo(m_scriptName);
	const auto& fields = sci.m_ScriptFieldMap;
	for (const auto& [fieldName, field] : fields)
	{

		if (field.m_fieldType == ScriptFieldType::Float)
		{
			float value = GetFieldValue<float>(field.m_classField);
      ScriptFieldInstance<float> test{ field,value };
      m_scriptFieldInstList.emplace_back(test);
		}
		else if (field.m_fieldType == ScriptFieldType::Int)
		{
      int value = GetFieldValue<int>(field.m_classField);
      ScriptFieldInstance<int> test{ field,value };
      m_scriptFieldInstList.emplace_back(test);
		}
		else if (field.m_fieldType == ScriptFieldType::Double)
		{
      double value = GetFieldValue<double>(field.m_classField);
      ScriptFieldInstance<double> test{ field,value };
      m_scriptFieldInstList.emplace_back(test);
		}
		else if (field.m_fieldType == ScriptFieldType::DVec3)
		{
      GE::Math::dVec3 value = GetFieldValue<GE::Math::dVec3>(field.m_classField);
      ScriptFieldInstance<GE::Math::dVec3> test{ field,value };
      m_scriptFieldInstList.emplace_back(test);
		}
    else if (field.m_fieldType == ScriptFieldType::DeckManagerFT)
    {
      DeckManager value = GetFieldValue< DeckManager>(field.m_classField);
      //std::cout << value.m_deck.m_cards.size() << "\n";
      ScriptFieldInstance< DeckManager> test{ field,value };
      m_scriptFieldInstList.emplace_back(test);
    }
    else if (field.m_fieldType == ScriptFieldType::CharacterTypeFT)
    {
      CharacterType value = GetFieldValue<CharacterType>(field.m_classField);
      //std::cout << value.m_deck.m_cards.size() << "\n";
      ScriptFieldInstance<CharacterType> test{ field,value };
      m_scriptFieldInstList.emplace_back(test);
    }
		else if (field.m_fieldType == ScriptFieldType::IntArr)
		{
      std::vector<int> value = GetFieldValueArr<int>(sm->m_appDomain, field.m_classField);
      ScriptFieldInstance<std::vector<int>> test{ field,value };
      m_scriptFieldInstList.emplace_back(test);
		}

    else if (field.m_fieldType == ScriptFieldType::UIntArr)
    {
      std::vector<unsigned> value = GetFieldValueArr<unsigned>(sm->m_appDomain, field.m_classField);
      ScriptFieldInstance<std::vector<unsigned>> test{ field,value };
      m_scriptFieldInstList.emplace_back(test);
    }
	}
}


void ScriptInstance::SetAllFields()
{
  GE::MONO::ScriptManager* sm = &GE::MONO::ScriptManager::GetInstance();
  ScriptClassInfo sci = sm->GetScriptClassInfo(m_scriptName);

  for (rttr::variant& f : m_scriptFieldInstList)
  {
    rttr::type dataType{ f.get_type() };
    if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<float>>()))
    {
      GE::MONO::ScriptFieldInstance<float>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<float>>();
      SetFieldValue<float>(sfi.m_data,sfi.m_scriptField.m_classField);
    }
    else if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<int>>()))
    {
      GE::MONO::ScriptFieldInstance<int>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<int>>();
      SetFieldValue<int>(sfi.m_data, sfi.m_scriptField.m_classField);
    }
    else if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<double>>()))
    {
      GE::MONO::ScriptFieldInstance<double>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<double>>();
      SetFieldValue<double>(sfi.m_data, sfi.m_scriptField.m_classField);
    }
    else if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<DeckManager>>()))
    {
      GE::MONO::ScriptFieldInstance<DeckManager>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<DeckManager>>();
      GE::MONO::ScriptInstance& deck = sfi.m_data.m_deckInstance;
      GE::MONO::ScriptInstance& deckMan = sfi.m_data.m_deckManagerInstance;

      deck.SetAllFields();
      deckMan.SetAllFields();
    }
    else if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<CharacterType>>()))
    {
      GE::MONO::ScriptFieldInstance<CharacterType>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<CharacterType>>();
      SetFieldValue<CharacterType>(sfi.m_data, sfi.m_scriptField.m_classField);
    }
    else if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<GE::Math::dVec3>>()))
    {
      GE::MONO::ScriptFieldInstance<GE::Math::dVec3>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<GE::Math::dVec3>>();
      SetFieldValue<GE::Math::dVec3>(sfi.m_data, sfi.m_scriptField.m_classField);
    }

    else if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<std::vector<int>>>()))
    {
      GE::MONO::ScriptFieldInstance<std::vector<int>>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<std::vector<int>>>();
      SetFieldValueArr<int>(sfi.m_data, sm->m_appDomain, sfi.m_scriptField.m_classField);
    }
    else if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<std::vector<unsigned>>>()))
    {
      GE::MONO::ScriptFieldInstance<std::vector<unsigned>>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<std::vector<unsigned>>>();
      SetFieldValueArr<unsigned>(sfi.m_data, sm->m_appDomain, sfi.m_scriptField.m_classField);
    }

  }

}



void ScriptInstance::GetAllUpdatedFields()
{
  GE::MONO::ScriptManager* sm = &GE::MONO::ScriptManager::GetInstance();
  ScriptClassInfo sci = sm->GetScriptClassInfo(m_scriptName);

  for (rttr::variant& f: m_scriptFieldInstList)
  {
    rttr::type dataType{ f.get_type() };
    if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<float>>())) 
    {
      GE::MONO::ScriptFieldInstance<float>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<float>>();
      sfi.m_data = GetFieldValue<float>(sfi.m_scriptField.m_classField);
    }
    else if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<int>>()))
    {
      GE::MONO::ScriptFieldInstance<int>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<int>>();
      sfi.m_data = GetFieldValue<int>(sfi.m_scriptField.m_classField);
    }
    else if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<double>>())) 
    {
      GE::MONO::ScriptFieldInstance<double>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<double>>();
      sfi.m_data = GetFieldValue<double>(sfi.m_scriptField.m_classField);
    }
    else if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<DeckManager>>()))
    {

      //Get the whole Deck Manager as a mnoObject
      GE::MONO::ScriptFieldInstance<DeckManager>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<DeckManager>>();

      //Check if we have created a cache for the deck manager. We will create if we didnt
      if (!sfi.m_data.m_deckManagerInstance.m_classInst)
      {
        sfi.m_data.m_deckManagerInstance.m_classInst = mono_field_get_value_object(sm->m_appDomain, sfi.m_scriptField.m_classField, m_classInst); //Get the mono data of the deck manager
        sfi.m_data.m_deckManagerInstance.m_scriptClass = mono_object_get_class(sfi.m_data.m_deckManagerInstance.m_classInst);

        MonoClassField* deckfield = mono_class_get_field_from_name(sfi.m_data.m_deckManagerInstance.m_scriptClass, "m_deck"); //Get the mono data of the deck 
        sfi.m_data.m_deckInstance.m_classInst = mono_field_get_value_object(sm->m_appDomain, deckfield, sfi.m_data.m_deckManagerInstance.m_classInst);
        sfi.m_data.m_deckInstance.m_scriptClass = mono_object_get_class(sfi.m_data.m_deckInstance.m_classInst);

        sfi.m_data.m_deckManagerInstance.m_scriptFieldInstList.push_back(ScriptFieldInstance<std::vector<Component::Card::CardID>>(ScriptField(UIntArr, "m_hand", mono_class_get_field_from_name(sfi.m_data.m_deckManagerInstance.m_scriptClass, "m_hand"))));
        sfi.m_data.m_deckManagerInstance.m_scriptFieldInstList.push_back(ScriptFieldInstance<std::vector<Component::Card::CardID>>(ScriptField(UIntArr, "m_queue", mono_class_get_field_from_name(sfi.m_data.m_deckManagerInstance.m_scriptClass, "m_queue"))));
        sfi.m_data.m_deckManagerInstance.m_scriptFieldInstList.push_back(ScriptFieldInstance<std::vector<Component::Card::CardID>>(ScriptField(UIntArr, "m_discardDisplay", mono_class_get_field_from_name(sfi.m_data.m_deckManagerInstance.m_scriptClass, "m_discardDisplay"))));
        sfi.m_data.m_deckInstance.m_scriptFieldInstList.push_back(ScriptFieldInstance<std::vector<Component::Card::CardID>>(ScriptField(UIntArr, "m_cards", mono_class_get_field_from_name(sfi.m_data.m_deckInstance.m_scriptClass, "m_cards"))));
        sfi.m_data.m_deckInstance.m_scriptFieldInstList.push_back(ScriptFieldInstance<std::vector<Component::Card::CardID>>(ScriptField(UIntArr, "m_drawOrderDisplay", mono_class_get_field_from_name(sfi.m_data.m_deckInstance.m_scriptClass, "m_drawOrderDisplay"))));
      }

      for (rttr::variant& dm : sfi.m_data.m_deckManagerInstance.m_scriptFieldInstList)
      {
        GE::MONO::ScriptFieldInstance<std::vector<Component::Card::CardID>>& dmSFI = dm.get_value<GE::MONO::ScriptFieldInstance<std::vector<Component::Card::CardID>>>();
        dmSFI.m_data = sfi.m_data.m_deckManagerInstance.GetFieldValueArr<GE::Component::Card::CardID>(sm->m_appDomain, dmSFI.m_scriptField.m_classField);
        if (dmSFI.m_scriptField.m_fieldName == "m_hand")
          sfi.m_data.m_hand = dmSFI.m_data;
        if (dmSFI.m_scriptField.m_fieldName == "m_queue")
          sfi.m_data.m_queue = dmSFI.m_data;
        if (dmSFI.m_scriptField.m_fieldName == "m_discardDisplay")
          sfi.m_data.m_discardDisplay = dmSFI.m_data;
      }

      for (rttr::variant& di : sfi.m_data.m_deckInstance.m_scriptFieldInstList)
      {
        GE::MONO::ScriptFieldInstance<std::vector<Component::Card::CardID>>& dSFI = di.get_value<GE::MONO::ScriptFieldInstance<std::vector<Component::Card::CardID>>>();
        dSFI.m_data = sfi.m_data.m_deckInstance.GetFieldValueArr<Component::Card::CardID>(sm->m_appDomain, dSFI.m_scriptField.m_classField);
        if (dSFI.m_scriptField.m_fieldName == "m_cards")
          sfi.m_data.m_deck.m_cards = dSFI.m_data;
        if (dSFI.m_scriptField.m_fieldName == "m_drawOrderDisplay")
          sfi.m_data.m_deck.m_drawOrderDisplay = dSFI.m_data;
      }  
    }
    else if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<CharacterType>>()))
    {
      GE::MONO::ScriptFieldInstance<CharacterType>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<CharacterType>>();
      sfi.m_data = GetFieldValue<CharacterType>(sfi.m_scriptField.m_classField);
    }
    else if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<GE::Math::dVec3>>()))
    {
      GE::MONO::ScriptFieldInstance<GE::Math::dVec3>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<GE::Math::dVec3>>();
      sfi.m_data = GetFieldValue<GE::Math::dVec3>(sfi.m_scriptField.m_classField);
    }

    else if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<std::vector<int>>>())) 
    {
      GE::MONO::ScriptFieldInstance<std::vector<int>>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<std::vector<int>>>();
      sfi.m_data = GetFieldValueArr<int>(sm->m_appDomain,sfi.m_scriptField.m_classField);
    }
    else if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<std::vector<unsigned>>>())) 
    {
      GE::MONO::ScriptFieldInstance<std::vector<unsigned>>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<std::vector<unsigned>>>();
      sfi.m_data = GetFieldValueArr<unsigned>(sm->m_appDomain, sfi.m_scriptField.m_classField);
    }

  }
  
}
