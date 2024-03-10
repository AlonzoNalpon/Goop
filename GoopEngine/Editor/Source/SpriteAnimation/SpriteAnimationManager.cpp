/*!*********************************************************************
\file   SpriteAnimationManager.cpp
\author a.nalpon\@digipen.edu
\date   16-September-2023
\brief  This file contains the implementation of the SpriteAnimationManager
class.
  
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include <SpriteAnimation/SpriteAnimationManager.h>
#include <map>
namespace GE::Graphics
{

  size_t SpriteAnimationManager::GetAnimID(std::string const& name) const noexcept
  {
    auto it{ m_animLookupTable.find(name) }; // attempt to find ID
#ifdef INVALID_VALUE_CHECK
    if (it == m_animLookupTable.end()) 
    {
      std::string error{ "No such animation with matching name exists: " };
    }
#endif
    return it == m_animLookupTable.end() ? BAD_OBJ_ID : it->second; // return ID or BAD_OBJ_ID if not found
  }

  std::string SpriteAnimationManager::GetAnimName(gObjID ID) const noexcept
  {
    auto it{ m_animID_NameLT.find(ID) };
    if (it == m_animID_NameLT.end())
    {
      std::string error{ "No such animation with matching ID exists: " };
      return std::string{};
    }
    return it->second;
  }

  SpriteAnimation const& SpriteAnimationManager::GetAnim(size_t id) const
  {
    // That's right. We're throwing if you give a bad ID
    if (id >= m_spriteAnimations.size())
    {
      throw Debug::Exception<SpriteAnimationManager>(Debug::LEVEL_ERROR, ErrMsg("Unable to get SpriteAnimation of id " + std::to_string(id)));
    }

    return m_spriteAnimations[id];
  }

  gObjID SpriteAnimationManager::CreateAnim(SpriteAnimation const& animation, std::string const& name)
  {
    auto it{ m_animLookupTable.find(name) };

    // If another animation with matching name has been found, notify user and replace
    if (it != m_animLookupTable.end())
    {
      GE::Debug::ErrorLogger::GetInstance().LogMessage(
        "SpriteAnimationManager: Animation " + name + " already exists, replacing animation.");
      /*std::string error{ "Another animation of this name exists: " };
      (error += name) += " | replacing this animation ...";
       << error << std::endl;*/

      size_t animID{ it->second };
      m_spriteAnimations[animID] = animation; // replace this animation

      return animID;
    }
    
    // At this line, we would be creating a new unique animation ...
    size_t animID{ m_spriteAnimations.size() }; // get the new ID (before pushing)
    m_animLookupTable[name] = animID;           // add this ID to the lookup table
    m_animID_NameLT[animID] = name;             // and add this ID to the other lookup table
    m_spriteAnimations.emplace_back(animation);    // push the new animation

    return animID; // return the animation ID
  }

  std::map<std::string, size_t> const& SpriteAnimationManager::GetAnimLT() const noexcept
  {
    return m_animLookupTable;
  }

  void SpriteAnimationManager::ClearAnimations()
  {
    m_spriteAnimations.clear();
    m_animID_NameLT.clear();
    m_animLookupTable.clear();
  }


}
