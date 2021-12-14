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

import android.app.Activity;
import android.app.ProgressDialog;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.MotionEvent;
import android.graphics.PointF;
import android.util.FloatMath;
import android.util.DisplayMetrics;

public class TeighaDwgActivity extends Activity {

    private TeighaDwgView mView;
    private String mFile;
    private ProgressDialog mPd;
    
	private Handler mPdHandler = new Handler() {
	        @Override
	        public void handleMessage(Message msg) {
	        	mPd.dismiss();
	        	mView.onLoad();
	        }
	};

    @Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        mView = new TeighaDwgView(getApplication());
        setContentView(mView);
        
        TeighaDWGJni.init();

        Bundle extras = getIntent().getExtras();                      
        mFile  = extras.getString("file");
	    mPd = ProgressDialog.show(this, "Please wait", "Loading " + mFile + " file...", true, false);
		new Thread(new Runnable() {
		    public void run() {
		    	TeighaDWGJni.open(mFile);
		    	mPdHandler.sendEmptyMessage(0);
		    }
		}).start();
    }
    
    @Override public void finalize (){
    }

    @Override protected void onPause() {
        mView.onPause();

        super.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
    }
    
    @Override protected void onDestroy() {
    	super.onDestroy();
    	if (isFinishing()) {
    	  mView.onDestroy();
    	  TeighaDWGJni.close();
    	  TeighaDWGJni.finit();
    	}
    }

     // we can be in one of these 3 states
    private static final int NONE = 0;
    private static final int DRAG = 1;
    private static final int ZOOM = 2;
    private static final int ORBIT = 3;
    private int mTouchMode = NONE;
    // remember some things for zooming
    private PointF mTouchStart = new PointF();
    private PointF mTouchMid = new PointF();
    private float mTouchOldDist = 1f;
    private float mTouchOldRot = 0f;
    private float[] mTouchLastEvent = null;
    private long mTouchLastTime = -1;
    
    /**
    * Determine the space between the first two fingers
    */
    private float spacing(MotionEvent event) {
      float x = event.getX(0) - event.getX(1);
      float y = event.getY(0) - event.getY(1);
      return (float)Math.sqrt(x * x + y * y);
    }
 
    /**
    * Calculate the mid point of the first two fingers
    */
    private void midPoint(PointF point, MotionEvent event) {
      float x = event.getX(0) + event.getX(1);
      float y = event.getY(0) + event.getY(1);
      point.set(x / 2, y / 2);
    }
 
    /**
    * Calculate the degree to be rotated by.
    *
    * @param event
    * @return Degrees
    */
    private float rotation(MotionEvent event) {
      double delta_x = (event.getX(0) - event.getX(1));
      double delta_y = (event.getY(0) - event.getY(1));
      double radians = Math.atan2(delta_y, delta_x);
      return (float)Math.toDegrees(radians);
    }
    
    @Override public boolean onTouchEvent (MotionEvent event) {
      switch (event.getAction() & MotionEvent.ACTION_MASK) {
    	case MotionEvent.ACTION_DOWN:
    	  if (mView.onToolbarClick(event.getX(), event.getY()))
    		  return true;
          long thisTime = System.currentTimeMillis();
    	  mTouchStart.set(event.getX(), event.getY());
          if (thisTime - mTouchLastTime < 250 && mTouchMode == NONE && TeighaDWGJni.viewCanRotate()) {
        	// Double click
        	mTouchMode = ORBIT;
        	mTouchLastTime = -1;
          } else {
    	    mTouchMode = DRAG;
    	    mTouchLastTime = thisTime;
          }
    	  mTouchLastEvent = null;
    	break;
    	case MotionEvent.ACTION_POINTER_DOWN:
    	  mTouchOldDist = spacing(event);
    	  if (mTouchOldDist > 10f) {
    	    midPoint(mTouchMid, event);
    	    mTouchMode = ZOOM;
    	  }
    	  mTouchLastEvent = new float[4];
    	  mTouchLastEvent[0] = event.getX(0);
    	  mTouchLastEvent[1] = event.getX(1);
    	  mTouchLastEvent[2] = event.getY(0);
    	  mTouchLastEvent[3] = event.getY(1);
    	  mTouchOldRot = rotation(event);
    	break;
    	case MotionEvent.ACTION_UP:
    	case MotionEvent.ACTION_POINTER_UP:
    	  mTouchMode = NONE;
    	  mTouchLastEvent = null;
    	break;
    	case MotionEvent.ACTION_MOVE:
    	  if (mTouchMode == DRAG) {
    	    float dx = event.getX() - mTouchStart.x;
    	    float dy = event.getY() - mTouchStart.y;
    	    TeighaDWGJni.viewTranslate(dx, dy);
    	    mTouchStart.x += dx; mTouchStart.y += dy;
    	  } else if (mTouchMode == ORBIT) {
      	    float dx = event.getX() - mTouchStart.x;
      	    float dy = event.getY() - mTouchStart.y;
      	    final DisplayMetrics displayMetrics = new DisplayMetrics();
      	    getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
      	    float density = displayMetrics.density;
      	    TeighaDWGJni.viewOrbit((float)Math.toRadians(dx / density / 2), (float)Math.toRadians(dy / density / 2));
      	    mTouchStart.x += dx; mTouchStart.y += dy;    		    		  
    	  } else if (mTouchMode == ZOOM) {
    	    float newDist = spacing(event);
    	    if (newDist > 10f) {
    	      float scale = (newDist / mTouchOldDist);
    	      TeighaDWGJni.viewScale(scale);
    	      mTouchOldDist = newDist;
    	  }
    	  if (mTouchLastEvent != null && event.getPointerCount() == 3) {
    	    float newRot = rotation(event);
    	    float r = newRot - mTouchOldRot;
    	    if (TeighaDWGJni.viewCanRotate())
    	  	  TeighaDWGJni.viewRotate((float)Math.toRadians(r));
    	    mTouchOldRot = newRot;
    	  }
    	}
    	break;
      }
      return true;    	
    }
    
}

