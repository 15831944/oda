/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2021, Open Design Alliance (the "Alliance"). 
// All rights reserved. 
// 
// This software and its documentation and related materials are owned by 
// the Alliance. The software may only be incorporated into application 
// programs owned by members of the Alliance, subject to a signed 
// Membership Agreement and Supplemental Software License Agreement with the
// Alliance. The structure and organization of this software are the valuable  
// trade secrets of the Alliance and its suppliers. The software is also 
// protected by copyright law and international treaty provisions. Application  
// programs incorporating this software must include the following statement 
// with their copyright notices:
//   
//   This application incorporates Open Design Alliance software pursuant to a license 
//   agreement with Open Design Alliance.
//   Open Design Alliance Copyright (C) 2002-2021 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////
package com.opendesign.android;

import android.util.Log;

public class TeighaDWGJni {
    public enum LIBRARY {
        NONE, TEIGHA, ARCHITECTURE, CIVIL
    }
    public static LIBRARY loadedLibrary = LIBRARY.NONE;
    static {
        try {
            System.loadLibrary("teigha_civil_jni");
            loadedLibrary = LIBRARY.CIVIL;
        } catch (UnsatisfiedLinkError err1) {
          try {
            System.loadLibrary("teigha_architecture_jni");
            loadedLibrary = LIBRARY.ARCHITECTURE;
          } catch (UnsatisfiedLinkError err2) {
            try {
                System.loadLibrary("teigha_jni");
                loadedLibrary = LIBRARY.TEIGHA;
            } catch (UnsatisfiedLinkError err3) {
                Log.e("JNI", "WARNING: Could not load libteigha*_jni.so");
            }
          }
        }
    }
	public static native boolean init ();
	
	public static native boolean finit ();
	
	public static native boolean open ( String file );
	
	public static native boolean createRenderer ( int width, int height );
	
	public static native boolean renderFrame ();
	
	public static native boolean destroyRenderer ();
	
	public static native boolean close ();

	public static native boolean viewTranslate ( float xAxis, float yAxis );
	
	public static native boolean viewScale ( float sCoef );
	
	public static native boolean viewCanRotate ();
	
	public static native boolean viewRotate ( float rAngle );
	
	public static native boolean viewOrbit ( float xAxis, float yAxis );
	
	public static native int viewGetRenderMode ();
	
	public static native boolean viewSetRenderMode ( int nMode );
	
	public static native boolean viewRegen ();
	
	public static native boolean saveContext ( int saveContextValue );
}
