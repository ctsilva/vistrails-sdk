/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

import com.vistrails.sdk.*;

public class OvalAction extends Action
{
  public float radius;
  
  public OvalAction()
  {
    this.radius = 0.0f;
  }
  
  public String classname()
  {
    return "Oval";
  }
  
  public void save(PList list)
  {
    list.addFloat(this.radius);
  }

  public void load(PList list)
  {
    this.radius = list.get(0).asFloat();
  }
  
  public static class OvalActionCreator extends ActionCreator
  {
    public Action createAction()
    {
        return new OvalAction();
    }

    public String actionId()
    {
        return "Oval";
    }
  }
  
  public static ActionCreator Creator()
  {
    return new OvalActionCreator();
  }
}


