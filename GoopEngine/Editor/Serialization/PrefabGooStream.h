/*!*********************************************************************
\file   PrefabGooStream.h
\author chengen.lau\@digipen.edu
\date   25-September-2023
\brief  
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include "IGooStream.h"
#include "Serializer.h"
#include "../ObjectFactory/ObjectFactory.h"

namespace GE
{
  namespace Serialization
  {

    // GooStream for Prefabs
    class PrefabGooStream : public IGooIStream<GE::ObjectFactory::ObjectData>
    {
    public:
      // Ctor reading json file into stream
      PrefabGooStream(std::string const& json);

      // Read from a json file. Contents read are appended into GooStream.
      bool Read(std::string const& json) override;

      // Unloads contents into an object
      bool Unload(container_type& object) override;

      static void PrefabLoadTest()
      {
        PrefabGooStream pfs{ "Assets/Data/Prefabs/MineWorm.json" };
        if (!pfs) { return; }

        GE::ObjectFactory::ObjectData obj{};
        pfs.Unload(obj);

        for (auto const& i : obj.m_components)
        {
          std::cout << GE::ECS::componentsToString.at(i.first) << ":\n" << i.second << "\n";
        }
      }

    private:
      static const char* const JsonNameKey, *const JsonSystemsKey, *const JsonComponentsKey;
    };

  }
}