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
        return (Utils.GetPosition(Entity.ID));
      }
      set
      {
        Utils.SetPosition(Entity.ID,  value);
      }
    }

    public Vec3<double> Scale
    {
      get
      {
        return (Utils.GetScale(Entity.ID));
      }
      set
      {
        Utils.SetScale(Entity.ID,  value);
      }
    }

    public Vec3<double> Rotation
    {
      get
      {
        return (Utils.GetRotation(Entity.ID));
      }
      set
      {
        Utils.SetRotation(Entity.ID,  value);
      }
    }
  }



}
