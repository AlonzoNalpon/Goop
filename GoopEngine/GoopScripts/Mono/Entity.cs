using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Mono
{
  public class Entity
  {
    protected Entity() { ID = 0; }

    internal Entity(uint id)
    {
      ID = id;
    }

    public readonly uint ID;

    //public bool HasComponent<T>() where T : Component, new()
    //{
    //  Type componentType = typeof(T);
    //  return InternalCalls.Entity_HasComponent(ID, componentType);
    //}

    //public T GetComponent<T>() where T : Component, new()
    //{
    //  if (!HasComponent<T>())
    //    return null;

    //  T component = new T() { Entity = this };
    //  return component;
    //}

    //public Entity FindEntityByName(string name)
    //{
    //  ulong entityID = InternalCalls.Entity_FindEntityByName(name);
    //  if (entityID == 0)
    //    return null;

    //  return new Entity(entityID);
    //}



  }
}
