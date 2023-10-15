/*!*********************************************************************
\file   ComponentWrapper.h
\author chengen.lau\@digipen.edu
\date   29-September-2023
\brief  Definition of ComponentWrapper struct that is a wrapper class
        for serialized json data. The class was designed to simplify
        the process of extracting serialized data for components. The
        class stores the data of a component which can then be
        extracted later through the .Get() function based on the
        specified data type as template arguments.

        Example Usage:
          ComponentWrapper cw{ componentStr };

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

namespace GE
{
  namespace Serialization
  {
    // Reference to the underlying json data

    class ComponentWrapper
    {
    public:
      using ComponentData = rapidjson::Value const&;

      // Non-default ctor taking in a string containing 
      // a component's data in json format
      ComponentWrapper(std::string const& componentData);

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
      template<> std::queue<Math::dVec2> Get(const char* key) const;
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

  } // namespace Serialization
} // namespace GE
