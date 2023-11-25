/*!*********************************************************************
\file   Deserializer.h
\author chengen.lau\@digipen.edu
\date   1-November-2023
\brief  The class encapsulating functions related to deserialization.
        Currently only contains functions for the node tree and prefab
        for the prefab editor.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <rapidjson/document.h>
#include <string>
#include <rttr/type.h>
#include <AI/TreeManager.h>
#include <ObjectFactory/ObjectFactory.h>

#ifdef _DEBUG
std::ostream& operator<<(std::ostream& os, rttr::type const& type);
#endif

namespace GE
{
  namespace Serialization
  {
    // Static class containing functions related to deserialization
    class Deserializer
    {
    public:
      // pure static class
      Deserializer() = delete;

      static ObjectFactory::ObjectFactory::EntityDataContainer DeserializeScene(std::string const& filepath);

      static std::vector<AI::TreeTemplate> DeserializeTrees(std::string const& filename);

      static ObjectFactory::VariantPrefab DeserializePrefabToVariant(std::string const& json);

      /*!*********************************************************************
      \brief
        Reads from a json file of systems and its components and returns
        an std::vector of systems in their order specified together with
        the components to be registered to it

        Each element in the vector is an std::pair containing
        1. The name of the system
        2. The components that should be registered to it
           (in the form of a signature)
      \param json
          The file to read from
      \return
        std::vector of systems to their respective components
      ************************************************************************/
      static std::vector<std::pair<std::string, ECS::ComponentSignature>> DeserializeSystems(std::string const& json);

      /*!*********************************************************************
      \brief
        Variadic function to validate the format of a json file.
        It iterates through the document and validates that each
        key specified is of type equal to the rapidjson::Type passed in after
        that key in the argument list.
        keyCount is the number of key-value pairs to validate, meaning
        the number of args in the (...) should be = keyCount * 2
        Each key should be followed by its rapidjson::Type in the argument
        list. Note that if the value is null, it will be ignored.

        Example usage:
          For a file that contains the following key-value pairs:
            "Name": "string" and "Components": [ "bla", "bla" ]
          You would invoke the function with:
            ScanJsonFileForMembers(document, 2,
              "Name", rapidjson::kStringType,
              "Components", rapidjson::kArrayType)
      \param document
        The rapidjson::Document to validate
      \param keyCount
        The number of fields (key-value pairs) to check
      \param ...
        The comma-separated argument list in pairs of key followed by 
        its corresponding type i.e. [const char*, rapidjson::Type]
       
      \return
      ************************************************************************/
      static bool ScanJsonFileForMembers(rapidjson::Document const& document, unsigned keyCount, ...);

    private:

      static void DeserializeBasedOnType(rttr::variant& object, rapidjson::Value const& value);

      static void DeserializeClassTypes(rttr::instance object, rapidjson::Value const& value);

      static bool DeserializeOtherComponents(rttr::variant& compVar, rttr::type const& type, rapidjson::Value const& value);

      static void DeserializeComponent(rttr::variant& compVar, rttr::type const& compType, rapidjson::Value const& compJson);

      static rttr::variant DeserializeBasicTypes(rapidjson::Value const& value);

      static rttr::variant DeserializeElement(rttr::type const& valueType, rapidjson::Value const& value);

      static void DeserializeSequentialContainer(rttr::variant_sequential_view& view, rapidjson::Value const& value);

      static void DeserializeScriptFieldInstList(rttr::variant& object, rapidjson::Value const& value);

      static bool InvokeConstructor(rttr::variant& var, rttr::type const& compType, rapidjson::Value const& value);

      static rttr::variant TryDeserializeIntoInt(rapidjson::Value const& value);
    };

  } // namespace Serialization
} // namespace GE
