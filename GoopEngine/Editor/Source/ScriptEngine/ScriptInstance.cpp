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
  m_ctorType = SPECIAL_CTOR;
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
  m_ctorType = ENTITY_CTOR;
  std::vector<void*> params = { &entityID };
  GE::MONO::ScriptManager* sm = &GE::MONO::ScriptManager::GetInstance();
  m_scriptClass = sm->GetScriptClass(scriptName);
  m_classInst = sm->InstantiateClass(scriptName.c_str(), params);
  m_onUpdateMethod = mono_class_get_method_from_name(m_scriptClass, "OnUpdate", 1);
  m_gcHandle = mono_gchandle_new(m_classInst, true);
  GetFields();
  m_onCreateMethod = mono_class_get_method_from_name(m_scriptClass, "OnCreate", 0);
}

void ScriptInstance::ReloadScript()
{
  //Clear all the old values
  m_onCreateMethod = nullptr;
  m_onUpdateMethod = nullptr;
  m_classInst = nullptr;
  m_scriptClass = nullptr;
  m_scriptFieldInstList.clear();
  mono_gchandle_free(m_gcHandle);

  // Load in the new values
  GE::MONO::ScriptManager* sm = &GE::MONO::ScriptManager::GetInstance();
  m_scriptClass = sm->GetScriptClass(m_scriptName);

  switch (m_ctorType)
  {
    case DEFAULT_CTOR:
    {
      m_classInst = sm->InstantiateClass(m_scriptName.c_str());
      break;
    }
    case ENTITY_CTOR:
    {
      
      std::vector<void*> arg{ &m_entityID };
      m_classInst = sm->InstantiateClass(m_scriptName.c_str(), arg);
      break;
    }
  }
 
  m_gcHandle = mono_gchandle_new(m_classInst, true);
  m_onUpdateMethod = mono_class_get_method_from_name(m_scriptClass, "OnUpdate", 1);
  m_onCreateMethod = mono_class_get_method_from_name(m_scriptClass, "OnCreate", 0);
  GetFields();
  SetAllFields();
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
    mono_runtime_invoke(m_onCreateMethod, m_classInst,nullptr, nullptr);
  }
} 

void ScriptInstance::GetFields()
{
  m_scriptFieldInstList.clear();
  GE::MONO::ScriptManager* sm = &GE::MONO::ScriptManager::GetInstance();
  ScriptClassInfo sci = sm->GetScriptClassInfo(m_scriptName);
	const auto& fields = sci.m_ScriptFieldMap;
	for (const auto& [fieldName, field] : fields)
	{
    /*if (m_scriptName == "Stats")
      std::cout << "Load Field: " << fieldName << "\n";*/
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
      //if (m_scriptName == "Stats")
      //  std::cout << "Load Field: " << mono_field_get_name(field.m_classField) << "\n";
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

       ScriptFieldInstance<DeckManager> sfi{field};

       sfi.m_data.m_deckManagerInstance.m_classInst = mono_field_get_value_object(sm->m_appDomain, sfi.m_scriptField.m_classField, m_classInst); //Get the mono data of the deck manager
       if (sfi.m_data.m_deckManagerInstance.m_classInst)
       {
         sfi.m_data.m_deckManagerInstance.m_scriptClass = mono_object_get_class(sfi.m_data.m_deckManagerInstance.m_classInst);
         MonoClassField* deckfield = mono_class_get_field_from_name(sfi.m_data.m_deckManagerInstance.m_scriptClass, "m_deck"); //Get the mono data of the deck 
         sfi.m_data.m_deckInstance.m_classInst = mono_field_get_value_object(sm->m_appDomain, deckfield, sfi.m_data.m_deckManagerInstance.m_classInst);
         sfi.m_data.m_deckInstance.m_scriptClass = mono_object_get_class(sfi.m_data.m_deckInstance.m_classInst);

         sfi.m_data.m_deckManagerInstance.m_scriptFieldInstList.push_back(ScriptFieldInstance<std::vector<Component::Card::CardID>>(ScriptField(UIntArr, "m_hand", mono_class_get_field_from_name(sfi.m_data.m_deckManagerInstance.m_scriptClass, "m_hand"))));
         sfi.m_data.m_deckManagerInstance.m_scriptFieldInstList.push_back(ScriptFieldInstance<std::vector<Component::Card::CardID>>(ScriptField(UIntArr, "m_queue", mono_class_get_field_from_name(sfi.m_data.m_deckManagerInstance.m_scriptClass, "m_queue"))));
         sfi.m_data.m_deckManagerInstance.m_scriptFieldInstList.push_back(ScriptFieldInstance<std::vector<Component::Card::CardID>>(ScriptField(UIntArr, "m_discardDisplay", mono_class_get_field_from_name(sfi.m_data.m_deckManagerInstance.m_scriptClass, "m_discardDisplay"))));
         sfi.m_data.m_deckInstance.m_scriptFieldInstList.push_back(ScriptFieldInstance<std::vector<Component::Card::CardID>>(ScriptField(UIntArr, "m_cards", mono_class_get_field_from_name(sfi.m_data.m_deckInstance.m_scriptClass, "m_cards"))));
         sfi.m_data.m_deckInstance.m_scriptFieldInstList.push_back(ScriptFieldInstance<std::vector<Component::Card::CardID>>(ScriptField(UIntArr, "m_drawOrderDisplay", mono_class_get_field_from_name(sfi.m_data.m_deckInstance.m_scriptClass, "m_drawOrderDisplay"))));

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

      m_scriptFieldInstList.emplace_back(sfi);

    }
    else if (field.m_fieldType == ScriptFieldType::HealthBarFT)
    {
      //ScriptFieldInstance<HealthBar> sfi{ field };
      //sfi.m_data.m_HealthBarInst.m_classInst = mono_field_get_value_object(sm->m_appDomain, sfi.m_scriptField.m_classField, m_classInst); //Get the mono data of the deck manager
      //if (sfi.m_data.m_HealthBarInst.m_classInst)
      //{
      //  sfi.m_data.m_HealthBarInst.m_scriptClass = mono_object_get_class(sfi.m_data.m_HealthBarInst.m_classInst);
      //  sfi.m_data.m_HealthBarInst.m_scriptFieldInstList.push_back(ScriptFieldInstance<int>(ScriptField(Int, "m_health", mono_class_get_field_from_name(sfi.m_data.m_HealthBarInst.m_scriptClass, "m_health"))));
      //  sfi.m_data.m_HealthBarInst.m_scriptFieldInstList.push_back(ScriptFieldInstance<int>(ScriptField(Int, "m_maxHealth", mono_class_get_field_from_name(sfi.m_data.m_HealthBarInst.m_scriptClass, "m_maxHealth"))));
      //  sfi.m_data.m_HealthBarInst.m_scriptFieldInstList.push_back(ScriptFieldInstance<int>(ScriptField(Int, "healthBarUI", mono_class_get_field_from_name(sfi.m_data.m_HealthBarInst.m_scriptClass, "healthBarUI"))));

      //  for (rttr::variant& dm : sfi.m_data.m_HealthBarInst.m_scriptFieldInstList)
      //  {
      //    GE::MONO::ScriptFieldInstance<int>& dmSFI = dm.get_value<GE::MONO::ScriptFieldInstance<int>>();
      //    dmSFI.m_data = sfi.m_data.m_HealthBarInst.GetFieldValue<int>(dmSFI.m_scriptField.m_classField);
      //    if (dmSFI.m_scriptField.m_fieldName == "m_health")
      //      sfi.m_data.m_health = dmSFI.m_data;
      //    if (dmSFI.m_scriptField.m_fieldName == "m_maxHealth")
      //      sfi.m_data.m_maxHealth = dmSFI.m_data;
      //    if (dmSFI.m_scriptField.m_fieldName == "healthBarUI")
      //      sfi.m_data.m_healthBarUI = dmSFI.m_data;
      //  }

      //}
      //m_scriptFieldInstList.emplace_back(sfi);

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
    else if (field.m_fieldType == ScriptFieldType::String)
    {
      MonoString* value = GetFieldValue<MonoString*>(field.m_classField);
      std::string const str{ MonoStringToSTD(value) };
      ScriptFieldInstance<std::string> sfi{ field, str };
      m_scriptFieldInstList.emplace_back(sfi);
    }
    else if (field.m_fieldType == ScriptFieldType::CharacterAnimsFT)
    {
      ScriptFieldInstance<CharacterAnims> sfi{ field };
      if (!sfi.m_data.m_characterAnimsInst.m_classInst)
      {
        sfi.m_data.m_characterAnimsInst.m_classInst = mono_field_get_value_object(sm->m_appDomain, sfi.m_scriptField.m_classField, m_classInst); //Get the mono data of the deck manager

        if (sfi.m_data.m_characterAnimsInst.m_classInst)
        {
          sfi.m_data.m_characterAnimsInst.m_scriptClass = mono_object_get_class(sfi.m_data.m_characterAnimsInst.m_classInst);
          sfi.m_data.m_characterAnimsInst.m_scriptName = mono_class_get_name(sfi.m_data.m_characterAnimsInst.m_scriptClass);
          sfi.m_data.m_characterAnimsInst.GetFields();
        }
      }

      for (rttr::variant& dm : sfi.m_data.m_characterAnimsInst.m_scriptFieldInstList)
      {
        MONO::ScriptFieldInstance<std::string>& dmSFI = dm.get_value<MONO::ScriptFieldInstance<std::string>>();
        dmSFI.m_data = MonoStringToSTD(sfi.m_data.m_characterAnimsInst.GetFieldValue<MonoString*>(dmSFI.m_scriptField.m_classField));
      }
      m_scriptFieldInstList.emplace_back(sfi);
      //std::cout << "getfields size:" << sfi.m_data.m_characterAnimsInst.m_scriptFieldInstList.size() << "\n";
    }
	}
}


void ScriptInstance::SetAllFields()
{
  GE::MONO::ScriptManager* sm = &GE::MONO::ScriptManager::GetInstance();
  ScriptClassInfo sci = sm->GetScriptClassInfo(m_scriptName);

  for (rttr::variant& f : m_scriptFieldInstList)
  {
    if (f.is_type<GE::MONO::ScriptFieldInstance<float>>())
    {
      GE::MONO::ScriptFieldInstance<float>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<float>>();
      SetFieldValue<float>(sfi.m_data,sfi.m_scriptField.m_classField);
    }
    else if (f.is_type<GE::MONO::ScriptFieldInstance<int>>())
    {
      GE::MONO::ScriptFieldInstance<int>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<int>>();
      SetFieldValue<int>(sfi.m_data, sfi.m_scriptField.m_classField);
    }
    else if (f.is_type<GE::MONO::ScriptFieldInstance<double>>())
    {
      GE::MONO::ScriptFieldInstance<double>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<double>>();
      SetFieldValue<double>(sfi.m_data, sfi.m_scriptField.m_classField);
    }
    else if (f.is_type<GE::MONO::ScriptFieldInstance<std::string>>())
    {
      GE::MONO::ScriptFieldInstance<std::string>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<std::string>>();
      mono_field_set_value(m_classInst, sfi.m_scriptField.m_classField, STDToMonoString(sfi.m_data));
    }
    else if (f.is_type<GE::MONO::ScriptFieldInstance<DeckManager>>())
    {
      GE::MONO::ScriptFieldInstance<DeckManager>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<DeckManager>>();
      GE::MONO::ScriptInstance& deck = sfi.m_data.m_deckInstance;
      GE::MONO::ScriptInstance& deckMan = sfi.m_data.m_deckManagerInstance;

      deck.SetAllFields();
      deckMan.SetAllFields();
    }
    else if (f.is_type<GE::MONO::ScriptFieldInstance<HealthBar>>())
    {
      GE::MONO::ScriptFieldInstance<HealthBar>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<HealthBar>>();
      GE::MONO::ScriptInstance& healthBarI = sfi.m_data.m_healthBarInst;

      healthBarI.SetAllFields();
    }
    else if (f.is_type<GE::MONO::ScriptFieldInstance<CharacterAnims>>())
    {
      GE::MONO::ScriptFieldInstance<CharacterAnims>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<CharacterAnims>>();
      GE::MONO::ScriptInstance& charAnimsI = sfi.m_data.m_characterAnimsInst;

      charAnimsI.SetAllFields();
      //std::cout << "setfields size:" << sfi.m_data.m_characterAnimsInst.m_scriptFieldInstList.size() << "\n";
    }
    else if (f.is_type<GE::MONO::ScriptFieldInstance<CharacterType>>())
    {
      GE::MONO::ScriptFieldInstance<CharacterType>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<CharacterType>>();
      SetFieldValue<CharacterType>(sfi.m_data, sfi.m_scriptField.m_classField);
    }
    else if (f.is_type<GE::MONO::ScriptFieldInstance<GE::Math::dVec3>>())
    {
      GE::MONO::ScriptFieldInstance<GE::Math::dVec3>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<GE::Math::dVec3>>();
      SetFieldValue<GE::Math::dVec3>(sfi.m_data, sfi.m_scriptField.m_classField);
    }
    else if (f.is_type<GE::MONO::ScriptFieldInstance<std::vector<int>>>())
    {
      GE::MONO::ScriptFieldInstance<std::vector<int>>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<std::vector<int>>>();
      SetFieldValueArr<int>(sfi.m_data, sm->m_appDomain, sfi.m_scriptField.m_classField);
    }
    else if (f.is_type<GE::MONO::ScriptFieldInstance<std::vector<unsigned>>>())
    {
      GE::MONO::ScriptFieldInstance<std::vector<unsigned>>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<std::vector<unsigned>>>();
      SetFieldValueArr<unsigned>(sfi.m_data, sm->m_appDomain, sfi.m_scriptField.m_classField);
    }

  }

}

void ScriptInstance::SetEntityID(GE::ECS::Entity entityId)
{
  m_entityID = entityId;
  MonoClass* parent = mono_class_get_parent(m_scriptClass);
  if (std::string(mono_class_get_name(parent)) != "Entity") { return; }

  MonoMethod*  setEntityIDMethod = mono_class_get_method_from_name(parent, "SetEntityID", 1);
  std::vector<void*> params = { &entityId };
  mono_runtime_invoke(setEntityIDMethod, mono_gchandle_get_target(m_gcHandle), params.data(), nullptr);
}

void ScriptInstance::PrintAllField()
{
  // std::cout << "GAF\n";
  GE::MONO::ScriptManager* sm = &GE::MONO::ScriptManager::GetInstance();
  ScriptClassInfo sci = sm->GetScriptClassInfo(m_scriptName);

  for (rttr::variant& f : m_scriptFieldInstList)
  {
    rttr::type dataType{ f.get_type() };
    if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<float>>()))
    {
      GE::MONO::ScriptFieldInstance<float>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<float>>();
      std::cout << sfi.m_scriptField.m_fieldName << ": " << GetFieldValue<float>(sfi.m_scriptField.m_classField) << "\n";
    }
    else if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<int>>()))
    {


      GE::MONO::ScriptFieldInstance<int>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<int>>();
      std::cout << sfi.m_scriptField.m_fieldName << ": " << GetFieldValue<int>(sfi.m_scriptField.m_classField) << "\n";
    }
    else if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<double>>()))
    {
      GE::MONO::ScriptFieldInstance<double>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<double>>();
      std::cout << sfi.m_scriptField.m_fieldName << ": " << GetFieldValue<double>(sfi.m_scriptField.m_classField) << "\n";
    }
    //else if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<DeckManager>>()))
    //{

      //Get the whole Deck Manager as a mnoObject
    //  GE::MONO::ScriptFieldInstance<DeckManager>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<DeckManager>>();

    //  //Check if we have created a cache for the deck manager. We will create if we didnt
    //  if (!sfi.m_data.m_deckManagerInstance.m_classInst)
    //  {
    //    sfi.m_data.m_deckManagerInstance.m_classInst = mono_field_get_value_object(sm->m_appDomain, sfi.m_scriptField.m_classField, m_classInst); //Get the mono data of the deck manager
    //    sfi.m_data.m_deckManagerInstance.m_scriptClass = mono_object_get_class(sfi.m_data.m_deckManagerInstance.m_classInst);

    //    MonoClassField* deckfield = mono_class_get_field_from_name(sfi.m_data.m_deckManagerInstance.m_scriptClass, "m_deck"); //Get the mono data of the deck 
    //    sfi.m_data.m_deckInstance.m_classInst = mono_field_get_value_object(sm->m_appDomain, deckfield, sfi.m_data.m_deckManagerInstance.m_classInst);
    //    sfi.m_data.m_deckInstance.m_scriptClass = mono_object_get_class(sfi.m_data.m_deckInstance.m_classInst);

    //    sfi.m_data.m_deckManagerInstance.m_scriptFieldInstList.push_back(ScriptFieldInstance<std::vector<Component::Card::CardID>>(ScriptField(UIntArr, "m_hand", mono_class_get_field_from_name(sfi.m_data.m_deckManagerInstance.m_scriptClass, "m_hand"))));
    //    sfi.m_data.m_deckManagerInstance.m_scriptFieldInstList.push_back(ScriptFieldInstance<std::vector<Component::Card::CardID>>(ScriptField(UIntArr, "m_queue", mono_class_get_field_from_name(sfi.m_data.m_deckManagerInstance.m_scriptClass, "m_queue"))));
    //    sfi.m_data.m_deckManagerInstance.m_scriptFieldInstList.push_back(ScriptFieldInstance<std::vector<Component::Card::CardID>>(ScriptField(UIntArr, "m_discardDisplay", mono_class_get_field_from_name(sfi.m_data.m_deckManagerInstance.m_scriptClass, "m_discardDisplay"))));
    //    sfi.m_data.m_deckInstance.m_scriptFieldInstList.push_back(ScriptFieldInstance<std::vector<Component::Card::CardID>>(ScriptField(UIntArr, "m_cards", mono_class_get_field_from_name(sfi.m_data.m_deckInstance.m_scriptClass, "m_cards"))));
    //    sfi.m_data.m_deckInstance.m_scriptFieldInstList.push_back(ScriptFieldInstance<std::vector<Component::Card::CardID>>(ScriptField(UIntArr, "m_drawOrderDisplay", mono_class_get_field_from_name(sfi.m_data.m_deckInstance.m_scriptClass, "m_drawOrderDisplay"))));
    //  }

    //  for (rttr::variant& dm : sfi.m_data.m_deckManagerInstance.m_scriptFieldInstList)
    //  {
    //    GE::MONO::ScriptFieldInstance<std::vector<Component::Card::CardID>>& dmSFI = dm.get_value<GE::MONO::ScriptFieldInstance<std::vector<Component::Card::CardID>>>();
    //    dmSFI.m_data = sfi.m_data.m_deckManagerInstance.GetFieldValueArr<GE::Component::Card::CardID>(sm->m_appDomain, dmSFI.m_scriptField.m_classField);
    //    if (dmSFI.m_scriptField.m_fieldName == "m_hand")
    //      sfi.m_data.m_hand = dmSFI.m_data;
    //    if (dmSFI.m_scriptField.m_fieldName == "m_queue")
    //      sfi.m_data.m_queue = dmSFI.m_data;
    //    if (dmSFI.m_scriptField.m_fieldName == "m_discardDisplay")
    //      sfi.m_data.m_discardDisplay = dmSFI.m_data;
    //  }

    //  for (rttr::variant& di : sfi.m_data.m_deckInstance.m_scriptFieldInstList)
    //  {
    //    GE::MONO::ScriptFieldInstance<std::vector<Component::Card::CardID>>& dSFI = di.get_value<GE::MONO::ScriptFieldInstance<std::vector<Component::Card::CardID>>>();
    //    dSFI.m_data = sfi.m_data.m_deckInstance.GetFieldValueArr<Component::Card::CardID>(sm->m_appDomain, dSFI.m_scriptField.m_classField);
    //    if (dSFI.m_scriptField.m_fieldName == "m_cards")
    //      sfi.m_data.m_deck.m_cards = dSFI.m_data;
    //    if (dSFI.m_scriptField.m_fieldName == "m_drawOrderDisplay")
    //      sfi.m_data.m_deck.m_drawOrderDisplay = dSFI.m_data;
    //  }
    //}
    //else if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<HealthBar>>()))
    //{

    //  GE::MONO::ScriptFieldInstance<HealthBar>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<HealthBar>>();

    //  if (!sfi.m_data.m_healthBarInst.m_classInst)
    //  {
    //    sfi.m_data.m_healthBarInst.m_classInst = mono_field_get_value_object(sm->m_appDomain, sfi.m_scriptField.m_classField, m_classInst); //Get the mono data of the deck manager

    //    if (sfi.m_data.m_healthBarInst.m_classInst)
    //    {
    //      sfi.m_data.m_healthBarInst.m_scriptClass = mono_object_get_class(sfi.m_data.m_healthBarInst.m_classInst);

    //      sfi.m_data.m_healthBarInst.m_scriptFieldInstList.push_back(ScriptFieldInstance<int>(ScriptField(Int, "m_health", mono_class_get_field_from_name(sfi.m_data.m_healthBarInst.m_scriptClass, "m_health"))));
    //      sfi.m_data.m_healthBarInst.m_scriptFieldInstList.push_back(ScriptFieldInstance<int>(ScriptField(Int, "m_maxHealth", mono_class_get_field_from_name(sfi.m_data.m_healthBarInst.m_scriptClass, "m_maxHealth"))));
    //      sfi.m_data.m_healthBarInst.m_scriptFieldInstList.push_back(ScriptFieldInstance<int>(ScriptField(Int, "healthBarUI", mono_class_get_field_from_name(sfi.m_data.m_healthBarInst.m_scriptClass, "healthBarUI"))));
    //    }
    //  }

    //  for (rttr::variant& dm : sfi.m_data.m_healthBarInst.m_scriptFieldInstList)
    //  {
    //    GE::MONO::ScriptFieldInstance<int>& dmSFI = dm.get_value<GE::MONO::ScriptFieldInstance<int>>();
    //    dmSFI.m_data = sfi.m_data.m_healthBarInst.GetFieldValue<int>(dmSFI.m_scriptField.m_classField);
    //    if (dmSFI.m_scriptField.m_fieldName == "m_health")
    //      sfi.m_data.m_health = dmSFI.m_data;
    //    if (dmSFI.m_scriptField.m_fieldName == "m_maxHealth")
    //      sfi.m_data.m_maxHealth = dmSFI.m_data;
    //    if (dmSFI.m_scriptField.m_fieldName == "healthBarUI")
    //      sfi.m_data.m_healthBarUI = dmSFI.m_data;
    //  }
    //}

    else if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<CharacterType>>()))
    {
      GE::MONO::ScriptFieldInstance<CharacterType>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<CharacterType>>();
      std::cout << sfi.m_scriptField.m_fieldName << ": " << GetFieldValue<CharacterType>(sfi.m_scriptField.m_classField) << "\n";
    }
    //else if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<GE::Math::dVec3>>()))
    //{
    //  GE::MONO::ScriptFieldInstance<GE::Math::dVec3>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<GE::Math::dVec3>>();
    //  sfi.m_data = GetFieldValue<GE::Math::dVec3>(sfi.m_scriptField.m_classField);
    //}

    //else if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<std::vector<int>>>()))
    //{
    //  GE::MONO::ScriptFieldInstance<std::vector<int>>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<std::vector<int>>>();
    //  sfi.m_data = GetFieldValueArr<int>(sm->m_appDomain, sfi.m_scriptField.m_classField);
    //}
    //else if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<std::vector<unsigned>>>()))
    //{
    //  GE::MONO::ScriptFieldInstance<std::vector<unsigned>>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<std::vector<unsigned>>>();
    //  sfi.m_data = GetFieldValueArr<unsigned>(sm->m_appDomain, sfi.m_scriptField.m_classField);
    //}

  }
}

void ScriptInstance::GetAllUpdatedFields()
{
 // std::cout << "GAF\n";
  GE::MONO::ScriptManager* sm = &GE::MONO::ScriptManager::GetInstance();
  ScriptClassInfo sci = sm->GetScriptClassInfo(m_scriptName);

  for (rttr::variant& f: m_scriptFieldInstList)
  {
    if (f.is_type<GE::MONO::ScriptFieldInstance<float>>()) 
    {
      GE::MONO::ScriptFieldInstance<float>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<float>>();
      sfi.m_data = GetFieldValue<float>(sfi.m_scriptField.m_classField);
    }
    else if (f.is_type<GE::MONO::ScriptFieldInstance<int>>())
    {
      GE::MONO::ScriptFieldInstance<int>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<int>>();
     /* if (m_scriptName == "Stats")
        std::cout << "Get field: " << mono_field_get_name(sfi.m_scriptField.m_classField);*/
      sfi.m_data = GetFieldValue<int>(sfi.m_scriptField.m_classField);
    }
    else if (f.is_type<GE::MONO::ScriptFieldInstance<double>>())
    {
      GE::MONO::ScriptFieldInstance<double>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<double>>();
      sfi.m_data = GetFieldValue<double>(sfi.m_scriptField.m_classField);
    }
    else if (f.is_type<GE::MONO::ScriptFieldInstance<DeckManager>>())
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
    else if (f.is_type<GE::MONO::ScriptFieldInstance<HealthBar>>())
    {
      GE::MONO::ScriptFieldInstance<HealthBar>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<HealthBar>>();

      if (!sfi.m_data.m_healthBarInst.m_classInst)
      {
        sfi.m_data.m_healthBarInst.m_classInst = mono_field_get_value_object(sm->m_appDomain, sfi.m_scriptField.m_classField, m_classInst); //Get the mono data of the deck manager

        if (sfi.m_data.m_healthBarInst.m_classInst)
        {
          sfi.m_data.m_healthBarInst.m_scriptClass = mono_object_get_class(sfi.m_data.m_healthBarInst.m_classInst);

          sfi.m_data.m_healthBarInst.m_scriptFieldInstList.push_back(ScriptFieldInstance<int>(ScriptField(Int, "m_health", mono_class_get_field_from_name(sfi.m_data.m_healthBarInst.m_scriptClass, "m_health"))));
          sfi.m_data.m_healthBarInst.m_scriptFieldInstList.push_back(ScriptFieldInstance<int>(ScriptField(Int, "m_maxHealth", mono_class_get_field_from_name(sfi.m_data.m_healthBarInst.m_scriptClass, "m_maxHealth"))));
          sfi.m_data.m_healthBarInst.m_scriptFieldInstList.push_back(ScriptFieldInstance<int>(ScriptField(Int, "healthBarUI", mono_class_get_field_from_name(sfi.m_data.m_healthBarInst.m_scriptClass, "healthBarUI"))));
        }
      }

      for (rttr::variant& dm : sfi.m_data.m_healthBarInst.m_scriptFieldInstList)
      {
        GE::MONO::ScriptFieldInstance<int>& dmSFI = dm.get_value<GE::MONO::ScriptFieldInstance<int>>();
        dmSFI.m_data = sfi.m_data.m_healthBarInst.GetFieldValue<int>(dmSFI.m_scriptField.m_classField);
        if (dmSFI.m_scriptField.m_fieldName == "m_health")
          sfi.m_data.m_health = dmSFI.m_data;
        if (dmSFI.m_scriptField.m_fieldName == "m_maxHealth")
          sfi.m_data.m_maxHealth = dmSFI.m_data;
        if (dmSFI.m_scriptField.m_fieldName == "healthBarUI")
          sfi.m_data.m_healthBarUI = dmSFI.m_data;
      }
    }
    else if (f.is_type<GE::MONO::ScriptFieldInstance<CharacterType>>())
    {
      GE::MONO::ScriptFieldInstance<CharacterType>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<CharacterType>>();
      sfi.m_data = GetFieldValue<CharacterType>(sfi.m_scriptField.m_classField);
    }
    else if (f.is_type<GE::MONO::ScriptFieldInstance<std::string>>())
    {
      GE::MONO::ScriptFieldInstance<std::string>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<std::string>>();
      sfi.m_data = MonoStringToSTD(GetFieldValue<MonoString*>(sfi.m_scriptField.m_classField));
    }
    else if (f.is_type<GE::MONO::ScriptFieldInstance<GE::Math::dVec3>>())
    {
      GE::MONO::ScriptFieldInstance<GE::Math::dVec3>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<GE::Math::dVec3>>();
      sfi.m_data = GetFieldValue<GE::Math::dVec3>(sfi.m_scriptField.m_classField);
    }
    else if (f.is_type<GE::MONO::ScriptFieldInstance<std::vector<int>>>())
    {
      GE::MONO::ScriptFieldInstance<std::vector<int>>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<std::vector<int>>>();
      sfi.m_data = GetFieldValueArr<int>(sm->m_appDomain,sfi.m_scriptField.m_classField);
    }
    else if (f.is_type<GE::MONO::ScriptFieldInstance<std::vector<unsigned>>>())
    {
      GE::MONO::ScriptFieldInstance<std::vector<unsigned>>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<std::vector<unsigned>>>();
      sfi.m_data = GetFieldValueArr<unsigned>(sm->m_appDomain, sfi.m_scriptField.m_classField);
    }
    else if (f.is_type<MONO::ScriptFieldInstance<CharacterAnims>>())
    {
      auto& sfi = f.get_value<MONO::ScriptFieldInstance<CharacterAnims>>();

      if (!sfi.m_data.m_characterAnimsInst.m_classInst)
      {
        sfi.m_data.m_characterAnimsInst.m_classInst = mono_field_get_value_object(sm->m_appDomain, sfi.m_scriptField.m_classField, m_classInst); //Get the mono data of the deck manager

        if (sfi.m_data.m_characterAnimsInst.m_classInst)
        {
          sfi.m_data.m_characterAnimsInst.m_scriptClass = mono_object_get_class(sfi.m_data.m_characterAnimsInst.m_classInst);

          rttr::type const characterAnimType{ rttr::type::get<CharacterAnims>() };
          for (auto& prop : characterAnimType.get_properties())
          {
            sfi.m_data.m_characterAnimsInst.m_scriptFieldInstList.emplace_back(ScriptFieldInstance<std::string>(ScriptField(String, prop.get_name().to_string().c_str(), mono_class_get_field_from_name(sfi.m_data.m_characterAnimsInst.m_scriptClass, prop.get_name().to_string().c_str()))));
          }
        }
      }

      sfi.m_data.m_characterAnimsInst.GetAllUpdatedFields();
    }

  }
}
