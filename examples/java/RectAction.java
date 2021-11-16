/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

import com.vistrails.sdk.*;

public class RectAction extends Action
{
  public float l;
  public float w;
  
  public RectAction()
  {
    this.l = 0.0f;
    this.w = 0.0f;
  }
  
  public String classname()
  {
    return "Rect";
  }
  
  public void save(PList list)
  {
    list.addFloat(this.l);
    list.addFloat(this.w);
  }

  public void load(PList list)
  {
    this.l = list.get(0).asFloat();
    this.w = list.get(1).asFloat();
  }
  
  public static class RectActionCreator extends ActionCreator
  {
    public Action createAction()
    {
        return new RectAction();
    }

    public String actionId()
    {
        return "Rect";
    }
  }
  
  public static ActionCreator Creator()
  {
    return new RectActionCreator();
  }
}
