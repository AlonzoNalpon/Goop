using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Mono
{
  public class MonoBehaviour
  {
    protected uint m_entityID;
    public MonoBehaviour(uint entityID)
    {
      m_entityID = entityID;
      Console.WriteLine("ITSHERE\n");

    }
    //public virtual void Awake()
    //{
    //  // Logic for Awake
    //}

    //public virtual void Start()
    //{
    //  // Logic for Start
    //}

    //public virtual void Update()
    //{
    //  // Logic for Update
    //}

    //public virtual void LateUpdate()
    //{
    //  // Logic for lateUpdate
    //}



    // Add other lifecycle methods like FixedUpdate, LateUpdate, etc.
  }
}
