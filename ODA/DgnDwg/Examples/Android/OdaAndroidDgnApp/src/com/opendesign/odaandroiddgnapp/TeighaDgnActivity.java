/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2020, Open Design Alliance (the "Alliance"). 
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
//   Open Design Alliance Copyright (C) 2002-2020 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////
package com.opendesign.odaandroiddgnapp;

import android.app.Activity;
import android.app.ProgressDialog;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;

public class TeighaDgnActivity extends Activity {
    private TeighaDgnView mView;
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
          mView = new TeighaDgnView(getApplication());
          setContentView(mView);

          TeighaDGNJni.init();

          Bundle extras = getIntent().getExtras();                      
          mFile  = extras.getString("file");
        mPd = ProgressDialog.show(this, "Please wait", "Loading " + mFile + " file...", true, false);
      new Thread(new Runnable() {
          public void run() {
            TeighaDGNJni.open(mFile);
            mPdHandler.sendEmptyMessage(0);
          }
      }).start();
      }

      @Override public void finalize (){
      }

      @Override protected void onPause() {
          super.onPause();
          mView.onPause();
      }

      @Override protected void onResume() {
          super.onResume();
          mView.onResume();
      }

      @Override protected void onDestroy() {
        super.onDestroy();
        if (isFinishing()) {
          mView.onDestroy();
          TeighaDGNJni.close();
          TeighaDGNJni.finit();
        }
      }
}
