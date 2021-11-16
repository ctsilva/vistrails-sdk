/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

import com.vistrails.sdk.*;
import java.io.*;

public class Testing
{
  static 
  {
    try 
    {
      System.loadLibrary("JavaVisTrails");
      System.out.println("Successfully loaded JavaVisTrails library!");
    } 
    catch (UnsatisfiedLinkError e) 
    {
      System.err.println("Native code library failed to load. " +
         "See the chapter on Dynamic Linking Problems " +
         "in the SWIG Java documentation for help.\n" + 
         e);
      System.exit(1);
    }
  }

  public static void main(String[] args)
  {
    //get global factory to register our actions
    ActionFactory factory = ActionFactory.globalFactory();  
    factory.registerAction(OvalAction.Creator());
    factory.registerAction(RectAction.Creator());
    System.out.println("Registered Actions");
    
    //create new session
    Session session = new Session();
    System.out.println("Created Session");
    
    //set the user
    session.info().setString("user","info@vistrails.com");
    System.out.println("Set User: " + session.info().get("user").asChar());
    
    //create new focus
    SessionFocus focus = new SessionFocus(session);
    System.out.println("Created Focus");

    
    //record some actions
    focus.record(new OvalAction());
    focus.record(new RectAction());
    System.out.println("Recorded Actions.");
    
    //select parent of the last version    
    focus.selectParent();
    
    //record some more actions
    focus.record(new OvalAction());
    focus.record(new RectAction());

    //load vistrail from xml file
    XmlStorage storage = new XmlStorage("test_vt.xml");
    session.discard();
    session.setVistrail(new Vistrail());
    session.load(storage);
    System.out.println("Loaded from test_vt.xml");
    
    //record some more actions
    focus.record(new OvalAction());
    focus.record(new RectAction());
    System.out.println("Record more actions.");

    //save vistrail to xml
    XmlStorage storage_out = new XmlStorage("test_vt_out.xml");
    session.save(storage_out);
    System.out.println("Saved vistrail to test_vt_out.xml");
  }
}
