using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Mono
{
  public abstract class MonoBehaviour
  {
    public virtual void Awake()
    {
      // Logic for Awake
    }

    public virtual void Start()
    {
      // Logic for Start
    }

    public virtual void Update()
    {
      // Logic for Update
    }

    public virtual void LateUpdate()
    {
      // Logic for lateUpdate
    }



    // Add other lifecycle methods like FixedUpdate, LateUpdate, etc.
  }
}
