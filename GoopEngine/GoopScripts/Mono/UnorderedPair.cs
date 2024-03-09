/*!*********************************************************************
\file   UnorderedPair.cpp
\author chengen.lau\@digipen.edu
\date   04-February-2024
\brief  Definition of UnorderedPair class, which is a 2-element tuple
        or pair where the ordering of elements don't matter. Inherits
        from IEquatable<T> to define the comparison function. Can be
        used as a key for a Dictionary.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Mono
{
  public class UnorderedPair<T> : IEquatable<UnorderedPair<T>>
  {
    public T first { get; }
    public T second { get; }
    public UnorderedPair(T _first, T _second)
    {
      first = _first;
      second = _second;
    }

    public bool Equals(UnorderedPair<T> rhs)
    {
      if (ReferenceEquals(this, rhs))
      {
        return true;
      }

      if (ReferenceEquals(null, rhs))
      {
        return false;
      }

      return first.Equals(rhs.first) && second.Equals(rhs.second) || 
        first.Equals(rhs.second) && second.Equals(rhs.first);

    }

    public override int GetHashCode()
    {
      unchecked
      {
        return (first == null ? 0 : EqualityComparer<T>.Default.GetHashCode(first)) +
          (second == null ? 0 : EqualityComparer<T>.Default.GetHashCode(second));
      }
    }
  }
}
