/*!*********************************************************************
\file   SpriteAnimationManager.h
\author a.nalpon@digipen.edu
\date   16-September-2023
\brief  This file contains the definition of the SpriteAnimationManager
class.
  
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef SPRITE_ANIMATION_MANAGER_H
#define SPRITE_ANIMATION_MANAGER_H
#include <vector>
#include <map>
#include <string>
#include <SpriteAnimation/SpriteAnimation.h>
namespace Graphics 
{
  /*!
   * \class SpriteAnimationManager
   * \brief  
   * This class manages all unique instances of sprite animations
   */
  class SpriteAnimationManager
  {
  public:
    static size_t const           INVALID_ID;         //!< constant to represent invalid ID

  public: // METHODS
    /*!*********************************************************************
    \brief Gets animation ID with matching name
      .
    \params
      name  name of the animation to look for
    \return the ID of the animation, or SpriteAnimationManager::INVALID_ID if cannot be found
      
    ************************************************************************/
    size_t                        GetAnimID(std::string const& name) const noexcept;

    /*!*********************************************************************
    \brief Get a read-only reference to SpriteAnimation from ID.
      Will throw if accessing out of bounds.
    \params
      id  The ID of the animation
    \return The const& to the animation.
      
    ************************************************************************/
    SpriteAnimation const&        GetAnim(size_t id) const;

    /*!*********************************************************************
    \brief Creates a new animation to be stored. If another animation
    of the same name is found, it will be replaced and a message will be
    printed.
      .
    \params
      animation the animation data to add
      name
    \return the ID of the added animation
      
    ************************************************************************/
    size_t                        CreateAnim(SpriteAnimation const& animation, std::string const& name);
  private: // PRIVATE MEMBERS
    std::map<std::string, size_t> m_animLookupTable;  //!< a lookup to see name to ID
    std::vector<SpriteAnimation>  m_spriteAnimations; //!< all the unique sprite animations
  };

}
#endif