using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Serialization
{
  interface ISerializedObject
  {
    void DeserializeFrom(string data);

    string Serialize();
  }
}
