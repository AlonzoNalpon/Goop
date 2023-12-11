/*!*********************************************************************
\file   Deserializer.h
\author chengen.lau\@digipen.edu
\date   1-November-2023
\brief  The class encapsulating functions related to deserialization.
        Implementation makes use of RTTR reflection library. Applies a 
        standard algorithm of recursion to work for for sequential 
        containers with any type and size. This is so that not much 
        code has to be modified when any components/classes are changed.

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

      /*!*********************************************************************
      \brief
        Main function called to deserialize a scene file. Returns a vector
        of std::pair of entity ID and VariantEntity objects encapsulating
        the deserialized data read from the file. When a missing field or
        invalid type is encountered, the function logs an error to the
        console and skips the element.
      \param filepath
        The scene file to deserialize
      \return
        An std::vector<std::pair<EntityID,VariantEntity>> with the
        deserailized data
      ************************************************************************/
      static ObjectFactory::ObjectFactory::EntityDataContainer DeserializeScene(std::string const& filepath);

      /*!*********************************************************************
      \brief
        Deserializes data used for the behaviour tree for the enemy AI
      \param filename
        The file to deserialize
      \return
        An std::vector of AI::TreeTemplates with the data read from the file
      ************************************************************************/
      static std::vector<AI::TreeTemplate> DeserializeTrees(std::string const& filename);

      static ObjectFactory::VariantPrefab2 DeserializePrefabToVariant2(std::string const& json);

      /*!*********************************************************************
      \brief
        Deserializes a prefab from a json file into a VariantPrefab object.
        It contains the prefab name along with a vector of its components
        stored in an rttr::variant object.
      \param json
        The file containing the prefab data
      \return
        The VariantPrefab object
      ************************************************************************/
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
            ScanJsonFileForMembers(jsonValue, 2,
              "Name", rapidjson::kStringType,
              "Components", rapidjson::kArrayType)
      \param value
        The rapidjson::Value to validate
      \param keyCount
        The number of fields (key-value pairs) to check
      \param ...
        The comma-separated argument list in pairs of key followed by 
        its corresponding type i.e. [const char*, rapidjson::Type]
       
      \return
      ************************************************************************/
      static bool ScanJsonFileForMembers(rapidjson::Value const& value, unsigned keyCount, ...);

    private:

      /*!*********************************************************************
      \brief
        Main function being called when deserializing to classify the given
        rttr::variant's type along with its corresponding rapidjson::value.
        Based on the result, the respective helper function is called to
        proceed with the deserialization process
      \param object
        The object to read the data into
      \param value
        The json data corresponding to the object
      ************************************************************************/
      static void DeserializeBasedOnType(rttr::variant& object, rapidjson::Value const& value);

      /*!*********************************************************************
      \brief
        Helper function to deserialize a class/struct object. It iterates
        through the object's properties and then calls the respective
        function on it based on its type, until all of the object's data
        members' data are read from the file.
      \param object
        The instance of the class object
      \param value
        The json data corresponding to the object
      ************************************************************************/
      static void DeserializeClassTypes(rttr::instance object, rapidjson::Value const& value);

      /*!*********************************************************************
      \brief
        This function handles the deserializing of certain components that
        require extra/non-typical deserialization and hence cannot make use
        of the normal process. E.g. an additional field that needs to be 
        read differently.
      \param compVar
        The rttr::variant to deserialize into
      \param type
        The type of the object to deserialize
      \param value
        The json data corresponding to the object
      \return
        True if such a component was encountered and false otherwise.
      ************************************************************************/
      static bool DeserializeOtherComponents(rttr::variant& compVar, rttr::type const& type, rapidjson::Value const& value);

      /*!*********************************************************************
      \brief
        This function handles the deserialization of a component. Based on
        the string value of a component read from the file, an instance
        of the class will be constructed through RTTR and then passed on
        to other functions to deserialize its data members.
      \param compVar
        The rttr::variant to deserialize into
      \param compType
        The type of the component
      \param compJson
        The json data corresponding to the component
      ************************************************************************/
      static void DeserializeComponent(rttr::variant& compVar, rttr::type const& compType, rapidjson::Value const& compJson);

      /*!*********************************************************************
      \brief
        This function handles deserializing of basic types such as ints,
        floats, strings,, bools and doubles. The type is determined by
        the rapidjson::Value and a rttr::variant containing the object 
        is returned
      \param value
        The json data of the object
      \return
        The deserialized object encapsulated withint an rttr::variant
      ************************************************************************/
      static rttr::variant DeserializeBasicTypes(rapidjson::Value const& value);

      /*!*********************************************************************
      \brief
        Given a type and its rapidjson::Value, default constructs the object
        and reads the data into it
      \param valueType
        The type of the object
      \param value
        The json data of the object
      \return
        An rttr::variant containing the deserialized object
      ************************************************************************/
      static rttr::variant DeserializeElement(rttr::type const& valueType, rapidjson::Value const& value);

      /*!*********************************************************************
      \brief
        This function handles the deserialization of sequential containers
        (such as std::vector). It makes use of recursion to handle nested
        containers. This allows it to deserialized the object until it
        reaches the underlying element.
      \param view
        The sequential_view of the container
      \param value
        The json data of the container
      ************************************************************************/
      static void DeserializeSequentialContainer(rttr::variant_sequential_view& view, rapidjson::Value const& value);

      //static void DeserializeScriptFieldInstList(rttr::variant& object, rapidjson::Value const& value);

      //static bool InvokeConstructor(rttr::variant& var, rttr::type const& compType, rapidjson::Value const& value);

      /*!*********************************************************************
      \brief
        This function attempts to deserialize a rapidjson::Value into an
        int object. This is used to help distinguish between signed and 
        unsigned ints.
      \param value
        The json data of the object
      \return
        An rttr::variant containing the int object
      ************************************************************************/
      static rttr::variant TryDeserializeIntoInt(rapidjson::Value const& value);
    };

  } // namespace Serialization
} // namespace GE
