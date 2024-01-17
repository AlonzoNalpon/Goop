#ifndef TEXT_STRING
#define TEXT_STRING
#include <string>
namespace Graphics::Fonts
{
  class TextString
  {
  private:
    std::string m_text; //!< the text object itself
    bool m_dirty;       //!< has this text been updated recently

  public:
    TextString(std::string const& str = std::string{}) :
      m_text{ str },
      m_dirty{ true }
    {}

    inline TextString& operator=(std::string const& str)
    {
      m_text = str;
      m_dirty = true;
      return *this;
    }

    inline operator std::string &()
    {
      return m_text;
    }

    bool const& dirty{ m_dirty };
  };
}

#endif
