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


import java.io.File;
import java.io.FileFilter;

import android.app.Application;
import android.app.ListActivity;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;

public class Main extends ListActivity {

  private class DGNFile {
      private String mPath;
      private String mFile;
      private long mSize;

      private class DGNFileOpen implements View.OnClickListener{

        private String mPath;

        public DGNFileOpen(String path){
          mPath = path;
        }

        // @Override
        public void onClick(View v) {
          Intent myIntent = new Intent(getApplicationContext(), TeighaDgnActivity.class);
          myIntent.putExtra("file", mPath);
                startActivityForResult(myIntent, 0);
        }

      }

      public DGNFile (String path,
                  String file,
                  long size){
        mPath = path;
        mFile = file;
        mSize = size;
      }

      public View getView(int position, View convertView, ViewGroup parent){
        LayoutInflater inflater = (LayoutInflater)mApp.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        View view;
        view = inflater.inflate(R.layout.listrow, parent, false);

        TextView filename_txt = (TextView) view.findViewById(R.id.filename);
        filename_txt.setText(mFile);

        TextView size_txt = (TextView) view.findViewById(R.id.size);
        long size = mSize;
        String metric;
         if ( size > (1024*1024*1024) ){
           metric = "Gb";
           size = size / (1024*1024*1024);
         }else if ( size > (1024*1024)){
           metric = "Mb";
           size = size / (1024*1024);
         }
         else{
           metric = "kb";
           size = size / 1024;
         }
         size_txt.setText(size + metric);
         
         view.setOnClickListener(new DGNFileOpen(mPath));
      return view;
      }
    }

  public class DGNFileAdapter extends ArrayAdapter < DGNFile >{

      ProgressDialog mPd;
      private Handler mPdHandler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
              mPd.dismiss();
              notifyDataSetChanged();
            }
        };
      private Handler mDataHandler = new Handler() {
          @Override
          public void handleMessage(Message msg) {
            DGNFile new_file = (DGNFile)msg.obj;
            add(new_file);
          }
       };
      public DGNFileAdapter(Context context) {
          super(context, R.layout.listrow);
         }
       private void _readDir ( File dir){
         File [] files = dir.listFiles(new FileFilter(){

        // @Override
        public boolean accept(File pathname) {
          if ( pathname.getName().endsWith(".dgn") ||
               pathname.isDirectory() == true ){
            return true;
          }
          return false;
        }
         });
         if ( files != null ){
           for ( int index = 0 ; index < files.length; index ++ ){
             if ( files[index].isDirectory() == true ){
               _readDir(files[index]);
             }else{
               mDataHandler.sendMessage(mDataHandler.obtainMessage(0, new DGNFile (files[index].getAbsolutePath(), files[index].getName(), files[index].length())));
             }
           }
         }
       }

       private void _rebuildSync(){
          File sdcard = Environment.getExternalStorageDirectory();
          if ( sdcard.isDirectory()){
              _readDir(sdcard);
          }
       }
       
       public void rebuild(Context ctx){
       this.clear();
          mPd = ProgressDialog.show(ctx, "Please wait", "Finding files...", true, false);
       new Thread(new Runnable() {
          public void run() {
            _rebuildSync();
            mPdHandler.sendEmptyMessage(0);
          }
       }).start();
       }

      @Override
      public View getView(int position, View convertView, ViewGroup parent) {
        try{
          DGNFile item = (DGNFile)getItem(position);
            return item.getView(position, convertView, parent);
        }catch(Exception e){
          return null;
        }
      }
  }

  private DGNFileAdapter mListAdapter;
  private Application mApp;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        mApp = (Application)getApplicationContext();
        mListAdapter = new DGNFileAdapter(this);
     ListView list = getListView();
     list.setAdapter(mListAdapter);
    mListAdapter.rebuild(this);
    }
}
