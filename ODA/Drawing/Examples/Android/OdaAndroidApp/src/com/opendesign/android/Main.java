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

import java.io.File;
import java.io.FileFilter;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Map;

import android.app.Application;
import android.app.ListActivity;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.AssetManager;
import android.content.res.Resources;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;

public class Main extends ListActivity {
	
	private class DWGFile {
    	private String mPath;
    	private String mFile;
    	private long mSize;
    	
    	private class DWGFileOpen implements View.OnClickListener{
    		
    		private String mPath;
    		
    		public DWGFileOpen(String path){
    			mPath = path;
    		}

			// @Override
			public void onClick(View v) {
				Intent myIntent = new Intent(getApplicationContext(), TeighaDwgActivity.class);
				myIntent.putExtra("file", mPath);
	            startActivityForResult(myIntent, 0);
			}
    		
    	}
    	
	    public DWGFile (String path,
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
	     	
	     	view.setOnClickListener(new DWGFileOpen(mPath));
			return view;
	    }
    }

	public class DWGFileAdapter extends ArrayAdapter < DWGFile >{
		
		  ProgressDialog mPd;
		  boolean mReadRecurcevly;
	      ArrayList<String> mExtEntries;
		  
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
			    	DWGFile new_file = (DWGFile)msg.obj;
			    	add(new_file);
			    }
		   };
		  public DWGFileAdapter(Context context) {
		      super(context, R.layout.listrow);
		      mReadRecurcevly = false;
		      mExtEntries = new ArrayList<String>();
	      }
		  public void setReadRecursive(boolean rec)
		  {
			  mReadRecurcevly = rec;
		  }
		  public void setEnabledExtensions(ArrayList<String> arr)
		  {
			  mExtEntries = arr;
		  }
		  private void _readDir( File dir){
			   File [] files = dir.listFiles(new FileFilter(){

				// @Override
				public boolean accept(File pathname) {
					boolean bShowFile = false;
					for (String it : mExtEntries) {
						bShowFile |= pathname.getName().toLowerCase().endsWith(it);
				    }
					if ( bShowFile ||
						(pathname.isDirectory() == true && mReadRecurcevly)){
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
						   mDataHandler.sendMessage(mDataHandler.obtainMessage(0, new DWGFile (files[index].getAbsolutePath(), files[index].getName(), files[index].length())));
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
				DWGFile item = (DWGFile)getItem(position);
			    return item.getView(position, convertView, parent);
			}catch(Exception e){
				return null;
			}
		  }
	}

    private void copyFile(String filename) {
      final String rawExternalStorage = Environment.getExternalStorageDirectory().getAbsolutePath();
      File file = new File(rawExternalStorage + "/" + filename);
       if(file.exists()) 
         return;
      AssetManager assetManager = this.getAssets();
      InputStream in = null;
      OutputStream out = null;
      try {
        in = assetManager.open(filename);
        out = new FileOutputStream(file);

        byte[] buffer = new byte[1024];
        int read;
        while ((read = in.read(buffer)) != -1) {
          out.write(buffer, 0, read);
        }
        in.close();
        in = null;
        out.flush();
        out.close();
        out = null;
      } catch (Exception e) {
        Log.e("MainActivity", e.getMessage());
      }
    }
	private DWGFileAdapter mListAdapter;
	private Application mApp;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        //copy adinit.dat file from assets to external storage
        CopyFileTask copyTask = new CopyFileTask();
        copyTask.execute();

        mApp = (Application)getApplicationContext();

        mListAdapter = new DWGFileAdapter(this);
	 	ListView list = getListView();
	 	list.setAdapter(mListAdapter);
    }

    @Override
    protected void onResume() {
        super.onResume();

        // Read preferences
        Resources res = getBaseContext().getResources();
        SharedPreferences SP = PreferenceManager.getDefaultSharedPreferences(getBaseContext());

        boolean readRecursefly = false;
        String fileExt;
        if (TeighaDWGJni.loadedLibrary == TeighaDWGJni.LIBRARY.TEIGHA) {
            fileExt = Arrays.toString(res.getStringArray(R.array.file_extension_default));
        } else {
            fileExt = Arrays.toString(res.getStringArray(R.array.file_extension_defaultA));
        }

        Map<String, ?> allEntries = SP.getAll();
        for (Map.Entry<String, ?> entry : allEntries.entrySet()) {
            if (MainSettings.KEY_FOLDER_DEPTH.equals(entry.getKey())) {
                readRecursefly = Boolean.valueOf(entry.getValue().toString());
            }else if(MainSettings.KEY_SELECTED_EXT.equals(entry.getKey()))
            {
                fileExt = entry.getValue().toString();
            }
        }

        ArrayList<String> fileExtArrayValues = new ArrayList<String>();
        fileExt = fileExt.replace(" ", "").replace("[","").replace("]","");
        String[] extEntries;
        String[] extValues;
        if (TeighaDWGJni.loadedLibrary == TeighaDWGJni.LIBRARY.TEIGHA) {
            extEntries = res.getStringArray(R.array.file_extension_entries);
            extValues = res.getStringArray(R.array.file_extension_values);
        } else {
            extEntries = res.getStringArray(R.array.file_extension_entriesA);
            extValues = res.getStringArray(R.array.file_extension_valuesA);
        }
        for (String it : Arrays.asList(fileExt.split(","))) {
           fileExtArrayValues.add(extEntries[Integer.valueOf(extValues[Integer.valueOf(it)])]);
        }
        mListAdapter.setReadRecursive(readRecursefly);
        mListAdapter.setEnabledExtensions(fileExtArrayValues);
        mListAdapter.rebuild(this);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main_menu, menu);//Menu Resource, Menu
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.settings:
                Intent settingsActivity = new Intent(getBaseContext(),
                        MainSettings.class);
                startActivity(settingsActivity);
            return true;
              default:
                return super.onOptionsItemSelected(item);  
        }
    }

    public class CopyFileTask extends AsyncTask<Void, Void, Void> {
      @Override
      protected Void doInBackground(Void... params) {
        copyFile("adinit.dat");
        return null;
      }
    }
}