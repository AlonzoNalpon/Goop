using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Mono
{
  public abstract class Component
	{
		public Entity Entity { get; internal set; }
	}

  public class Transform : Component
  {
    public Vec3<double> Position
    {
      get
      {
        return (Utils.GetPosition(Entity.entityID));
      }
      set
      {
        Utils.SetPosition(Entity.entityID,  value);
      }
    }

    public Vec3<double> Scale
    {
      get
      {
        return (Utils.GetScale(Entity.entityID));
      }
      set
      {
        Utils.SetScale(Entity.entityID,  value);
      }
    }

    public Vec3<double> Rotation
    {
      get
      {
        return (Utils.GetRotation(Entity.entityID));
      }
      set
      {
        Utils.SetRotation(Entity.entityID,  value);
      }
    }
  }



}
