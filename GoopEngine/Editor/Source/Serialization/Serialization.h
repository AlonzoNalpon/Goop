/*!*********************************************************************
\file   Serialization.h
\author chengen.lau\@digipen.edu
\date   29-September-2023
\brief  Definition of ComponentWrapper struct that is a wrapper class
        for serialized json data. The class was designed to simplify
        the process of extracting serialized data for components. The
        class stores the data of a component which can then be 
        extracted later through the GetData() function which returns
        a reference to the underlying Document object.

        Example Usage:
          ComponentWrapper cw{ componentStr };
          ComponentData const& data{ cw.GetData() };

        To extract the data, apply subscript operator with key
        Then, call Get___() where the blank is the type to extract
        the value in.
          - e.g. data["keyName"].GetInt() or data["keyName"].GetDouble()  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <rapidjson/document.h>
#include <string>

namespace GE
{
  namespace Serialization
  {
    // Reference to the underlying json data
    using ComponentData = rapidjson::Value const&;

    class ComponentWrapper
    {
    public:
      // Non-default ctor taking in a string containing 
      // a component's data in json format
      ComponentWrapper(std::string const& componentData)
      {
        if (m_data.Parse(componentData.c_str()).HasParseError())
        {
          throw Debug::Exception<ComponentWrapper>(Debug::LEVEL_ERROR, ErrMsg("Unable to parse component data"));
        }
      }

      /*!*********************************************************************
      \brief
        Returns a reference to the component data in json format
      \return
        Reference to the underlying rapidjson::Value object
      ************************************************************************/
      inline ComponentData const& GetData() const noexcept { return m_data.MemberBegin()->value; }

      /*!*********************************************************************
      \brief
        Returns the number of elements in the serialized component
      \return
        Number of elements in the serialized component
      ************************************************************************/
      inline rapidjson::SizeType GetCount() const noexcept { return m_data[0].MemberCount(); }

    private:
      rapidjson::Document m_data;
    };
  }
}
