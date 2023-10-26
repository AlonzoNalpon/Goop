/*!*********************************************************************
\file   TextureManager.h
\author a.nalpon@digipen.edu
\date   29-September-2023
\brief  This file contains the definition for the TextureManager class
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef TEXTURE_MANAGER
#define TEXTURE_MANAGER
#include <Graphics/Def/GraphicsTypes.h>
#include <Graphics/Texture.h>
#include <vector>
#include <map>
// a better alternative to automatically storing and deleting textures coming soon
namespace Graphics {
  class TextureManager{
  public:
    /*!*********************************************************************
    \brief
      Add a new texture object to manager. Will not work if OpenGL is not
      initialized
    \params
      name name of the texture
      w width of the texture
      h height of the texture
      imageData the image data
    \return the ID of the texture
      
    ************************************************************************/
    GLuint AddTexture(std::string const& name, GLint w, GLint h, unsigned char const* imageData);

    void DestroyTexture(GLuint textureID);
    //void DestroyTextures(); //!< not implemented yet

    /*!*********************************************************************
    \brief
      Get a read-only reference to corresponding texture from ID.
    \params
      id  the id of the texture
    \return
      
    ************************************************************************/
    Texture const& GetTexture(GLuint id) const;

    /*!*********************************************************************
    \brief
      Gets the texture ID from name.
    \params
      name the name of the texture
    \return the ID of the texture, BAD_OBJ_ID if no such texture was found
      
    ************************************************************************/
    GLuint GetTextureID(std::string const& name) const;

    /*!*********************************************************************
    \brief
      Gets the texture name from ID.
    \params
      id the ID of the texture
    \return the ID of the texture, BAD_OBJ_ID if no such texture was found

    ************************************************************************/
    std::string GetTextureName(GLuint id) const;
  private:
    void DestroyTextureByName(std::string name);
    std::map<GLuint, Texture>     m_textures;     //!< container for textures
    std::map<std::string, GLuint> m_texturesLT;   //!< map for texture lookup table
    std::map<GLuint, std::string> m_texNameLT;    //!< map for ID to name lookup table
  };
}
#endif
