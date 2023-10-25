/*!*********************************************************************
\file   ComponentWrapper.h
\author chengen.lau\@digipen.edu
\date   29-September-2023
\brief  Definition of ComponentDWrapper struct that is a wrapper class
        for serialized json data. The class was designed to simplify
        the process of extracting serialized data for components. The
        class stores the data of a component which can then be
        extracted later through the .Get() function based on the
        specified data type as template arguments.

        Example Usage:
          ComponentDWrapper cw{ componentStr };

        To extract the data, simply call .Get<>() function and
        specify the data type you want to extract it in as template
        arguments.
          - e.g. cw.Get<int>("keyName") or cw.Get<double>("keyName")
            where "keyName" is the key of the value in the json file

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <rapidjson/document.h>
#include <Math/GEM.h>
#include <vector>
#include <string>
#include <sstream>
#include <Component/Velocity.h>
#include <rttr/type>

namespace GE
{
  namespace Serialization
  {
    // Reference to the underlying json data

    class ComponentDWrapper
    {
    public:
      using ComponentData = rapidjson::Value const&;

      // Non-default ctor taking in a string containing 
      // a component's data in json format
      ComponentDWrapper(std::string const& componentData);

      /*!*********************************************************************
      \brief
        Extracts the value of a given key based on the given type
      \param key
        The key of the value to extract
      \return
        The value corresponding to the given key as type T
      ************************************************************************/
      template <typename T>
      T Get(const char* key) const;
      template<> bool Get(const char* key) const;
      template<> double Get(const char* key) const;
      template<> int Get(const char* key) const;
      template<> unsigned Get(const char* key) const;
      template<> size_t Get(const char* key) const;
      template<> const char* Get(const char* key) const;
      template<> std::string Get(const char* key) const;
      template<> Math::Vec2 Get(const char* key) const;
      template<> Math::dVec2 Get(const char* key) const;
      template<> Math::dVec3 Get(const char* key) const;
      template<> std::queue<Math::dVec2> Get(const char* key) const;
      template<> std::queue<Math::dVec3> Get(const char* key) const;
      template<> std::vector<std::pair<std::string, std::string>>
                 Get(const char* key) const;
      template<> Component::DragForce Get(const char* key) const;
      template<> std::vector<Component::LinearForce> Get(const char* key) const;

      /*!*********************************************************************
      \brief
        Returns a reference to the component data in json format
      \return
        Reference to the underlying rapidjson::Value object
      ************************************************************************/
      //inline ComponentData const& GetData() const noexcept { return m_data.MemberBegin()->value; }

      /*!*********************************************************************
      \brief
        Returns the number of elements in the serialized component
      \return
        Number of elements in the serialized component
      ************************************************************************/
      inline rapidjson::SizeType GetCount() const noexcept { return m_data[0].MemberCount(); }

    private:
      rapidjson::Document m_data;
      rapidjson::Value const* m_ptr = nullptr;  // ptr to underlying json data
    };

    class ComponentSWrapper
    {
    public:
      ComponentSWrapper(const char* componentName);
       
      /*!*********************************************************************
      \brief
        Inserts a value with a basic type into the wrapper object.
        Supports most C standard types (int, float, double etc.)

        Additional supports:
          GE::Math::dVec3
      \param key
        The value of the key in string to store the value as
      \param value
        The value to serialize into the wrapper object
      ************************************************************************/
      void InsertBasicType(const char* key, rttr::variant value);

      /*template <typename T>
      void Insert(const char* key, T value);*/
    private:
      rapidjson::Document m_data;
    };

  } // namespace Serialization
} // namespace GE
