/*!*********************************************************************
\file   PrefabGooStream.h
\author chengen.lau\@digipen.edu
\date   25-September-2023
\brief  GooStream class used for deserializing of a prefab json file, 
        together with the systems it is under, it's individual
        components, and their values. The class overrides only input
        functions.

        For the format of the json file, you can refer to
        Assets/Data/Prefabs/MineWorm.json
        For more details on usage, refer to GooStream.h

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include "IGooStream.h"
#include "../ObjectFactory/ObjectStructs.h"

namespace GE
{
  namespace Serialization
  {

    // GooStream for Prefabs
    class PrefabGooStream : public IGooIStream<std::pair<std::string, GE::ObjectFactory::ObjectData>>
    {
    public:
      // Ctor reading json file into stream
      PrefabGooStream(std::string const& json);

      /*!*********************************************************************
      \brief
        Read from a json file. Contents read are appended into the stream.
        This function will check for the formatting for the file when reading
        its contents. It ensures that the data is nested properly and that
        the prefab contains the keys JsonNameKey, JsonSystemsKey and
        JsonComponentsKey based on the static const char* private variables.
      \param json
        The file to read from
      \return
        True if the operation succeeded and false otherwise
      ************************************************************************/
      bool Read(std::string const& json) override;

      /*!*********************************************************************
      \brief
        Unloads the contents of the stream into a container
      \param json
        The container to output to
      \return
        True if the operation succeeded and false otherwise
      ************************************************************************/
      bool Unload(container_type& object) override;

      // Test function for prefab loading
      static void PrefabLoadTest()
      {
        PrefabGooStream pfs{ "Assets/Data/Prefabs/MineWorm.json" };
        if (!pfs) { return; }

        std::pair<std::string, GE::ObjectFactory::ObjectData> obj{};
        pfs.Unload(obj);
        std::cout << obj.first << "\nComp Sig: " << obj.second.m_componentSignature 
          << "\nSys Sig: " << obj.second.m_systemSignature << "\n";
        std::cout << "Components:\n";
        for (auto const& i : obj.second.m_components)
        {
          std::cout << GE::ECS::componentsToString.at(i.first) << ":\n" << i.second << "\n";
        }
      }

    private:
      static const char JsonNameKey[], JsonSystemsKey[], JsonComponentsKey[];
    };

  }
}