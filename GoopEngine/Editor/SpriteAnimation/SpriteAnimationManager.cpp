/*!*********************************************************************
\file   SpriteAnimationManager.cpp
\author a.nalpon@digipen.edu
\date   16-September-2023
\brief  This file contains the implementation of the SpriteAnimationManager
class.
  
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <SpriteAnimation/SpriteAnimationManager.h>

namespace Graphics
{
  size_t const SpriteAnimationManager::INVALID_ID{ static_cast<size_t>(-1) }; // the highest possible value

  size_t SpriteAnimationManager::GetAnimID(std::string const& name) const noexcept
  {
    auto it{ m_animLookupTable.find(name) }; // attempt to find ID
#ifdef INVALID_VALUE_CHECK
    if (it == m_animLookupTable.end()) 
    {
      std::string error{ "No such animation with matching name exists: " };
      ERR_LOG_FILE(error + name);
    }
#endif
    return it == m_animLookupTable.end() ? INVALID_ID : it->second; // return ID or INVALID_ID if not found
  }

  SpriteAnimation const& SpriteAnimationManager::GetAnim(size_t id) const
  {
    return m_spriteAnimations[id]; // That's right. We're throwing if you give a bad ID
  }

  size_t SpriteAnimationManager::CreateAnim(SpriteAnimation const& animation, std::string const& name)
  {
    auto it{ m_animLookupTable.find(name) };

    // If another animation with matching name has been found, notify user and replace
    if (it != m_animLookupTable.end())
    {
      std::string error{ "Another animation of this name exists: " };
      (error += name) += " | replacing this animation ...";
      ERR_LOG_FILE(error);

      size_t animID{ it->second };
      m_spriteAnimations[animID] = animation; // replace this animation

      return animID;
    }
    
    // At this line, we would be creating a new unique animation ...
    size_t animID{ m_spriteAnimations.size() }; // get the new ID (before pushing)
    m_animLookupTable[name] = animID;           // add this ID to the lookup table
    m_spriteAnimations.emplace_back(animation);    // push the new animation

    return animID; // return the animation ID
  }


}