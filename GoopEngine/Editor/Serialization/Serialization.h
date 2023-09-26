#pragma once
#include <pch.h>
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
      // Returns a reference to the component data in json format
      inline ComponentData const& GetData() const noexcept { return m_data.MemberBegin()->value; }

      ComponentWrapper(std::string const& componentData)
      {
        if (m_data.Parse(componentData.c_str()).HasParseError())
        {
          throw Debug::Exception<ComponentWrapper>(Debug::LEVEL_ERROR, ErrMsg("Unable to parse component data"));
        }
      }

      inline rapidjson::SizeType GetCount() const noexcept { return m_data[0].MemberCount(); }

    private:
      rapidjson::Document m_data;
    };
  }
}
