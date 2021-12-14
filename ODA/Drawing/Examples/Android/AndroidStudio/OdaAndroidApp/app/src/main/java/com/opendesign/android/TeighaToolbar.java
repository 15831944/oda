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

import android.content.Context;
import android.opengl.GLES10;
import android.opengl.GLES20;
import android.util.Log;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

public final class TeighaToolbar {

	// Vertex shader
	private static final String vertexShaderSrc =
		"precision mediump float;\n" +
		"precision mediump int;\n" +
		"attribute vec4 a_VertPosition;\n" +
		"attribute vec2 a_TexCoord;\n" +
		"varying vec2 v_TexCoord;\n" +
		"void main()\n" +	
		"{\n" +	
		"  gl_Position = a_VertPosition;\n" +
		"  v_TexCoord = a_TexCoord;\n" +
		"}\n";
	// Fragment shader
	private static final String fragmentShaderSrc =
		"precision mediump float;\n" +
		"precision mediump int;\n" +
		"varying vec2 v_TexCoord;\n" +
		"uniform sampler2D u_TextureUnit;\n" +
		"void main()\n" +
		"{\n" +
		"  vec4 texColor = texture2D(u_TextureUnit, v_TexCoord);\n" +
		"  gl_FragColor.rgb = texColor.rgb;\n" +
		"  gl_FragColor.a = texColor.r;\n" +
		"}\n";
	
	private static final float BTN_SCALE = 0.1f;
	public static final int FLOAT_SZ = 4;
	
	private boolean mInitialized = false;
	private int mTextures[];
	// GLES2
	private int mVertexShader;
	private int mFragmentShader;
	private int mProgram;
	private int mVertexAttribute;
	private int mTexCoordAttribute;
	private int mTextureUniform;
	// FPS meter
	private float mFPS;
	private long mFrameTimef1;
	private long mFrameTimef2;
	private long mFrameCount1;
	private long mFrameCount2;
	// Settings
	private boolean mDrawFPS = true;
	private int mRenderMode = -1;
	private boolean mModelSpace = true;
	private int mTask = -1;
	
	public boolean click(float touchX, float touchY, int winWidthPx, int winHeightPx) {
		if (!mInitialized)
			return false;
		final float glX = touchX / winWidthPx * 2 - 1.0f;
		final float glY = 1.0f - touchY / winHeightPx * 2;
		final float btnSize = (float)Math.min(winWidthPx, winHeightPx) * BTN_SCALE;
		final float btnSizeX = btnSize / winWidthPx; 
		final float btnSizeY = btnSize / winHeightPx; 
		if (mModelSpace) {
			if (glY <= -1.0f + btnSizeY &&
				glX <= -1.0f + btnSizeX * 9) {
				mTask = (int)((glX + 1.0f) / btnSizeX);
				return true;
			}
		} else {
			if (glY <= -1.0f + btnSizeY &&
				glX <= -1.0f + btnSizeX * 2) {
				mTask = (int)((glX + 1.0f) / btnSizeX) + 7;
				return true;
			}			
		}
		return false;
	}
	
	public void tick(long delta) {
		mFrameTimef2 += delta;
		mFrameCount2++;
		if (mFrameTimef2 - mFrameTimef1 > 1000) {
			mFPS = (float)(mFrameCount2 - mFrameCount1) * 1000.0f / (mFrameTimef2 - mFrameTimef1);
			mFrameCount1 = mFrameCount2;
			mFrameTimef1 = mFrameTimef2;
		}
	}
	
	public void initialize(int GLESVER, Context ctx) {
		if (mInitialized)
			return;
		if (GLESVER == 2) {
			// Textures
			mTextures = new int[20];
			GLES20.glGenTextures(20, mTextures, 0);
			for (int nTex = 0; nTex < 20; nTex++) {
				GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mTextures[nTex]);
				GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
				GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
				ByteBuffer bb = ByteBuffer.allocate(32 * 32);
				switch (nTex) {
					case 0: bb.put(ReadFromfile("icon00Src.txt", ctx)); break;
					case 1: bb.put(ReadFromfile("icon01Src.txt", ctx)); break;
					case 2: bb.put(ReadFromfile("icon02Src.txt", ctx)); break;
					case 3: bb.put(ReadFromfile("icon03Src.txt", ctx)); break;
					case 4: bb.put(ReadFromfile("icon04Src.txt", ctx)); break;
					case 5: bb.put(ReadFromfile("icon05Src.txt", ctx)); break;
					case 6: bb.put(ReadFromfile("icon06Src.txt", ctx)); break;
					case 7: bb.put(ReadFromfile("icon07Src.txt", ctx)); break;
					case 8: bb.put(ReadFromfile("icon08Src.txt", ctx)); break;
					case 9: bb.put(ReadFromfile("icon09Src.txt", ctx)); break;
					case 10: bb.put(ReadFromfile("icon10Src.txt", ctx)); break;
					case 11: bb.put(ReadFromfile("icon11Src.txt", ctx)); break;
					case 12: bb.put(ReadFromfile("icon12Src.txt", ctx)); break;
					case 13: bb.put(ReadFromfile("icon13Src.txt", ctx)); break;
					case 14: bb.put(ReadFromfile("icon14Src.txt", ctx)); break;
					case 15: bb.put(ReadFromfile("icon15Src.txt", ctx)); break;
					case 16: bb.put(ReadFromfile("icon16Src.txt", ctx)); break;
					case 17: bb.put(ReadFromfile("icon17Src.txt", ctx)); break;
					case 18: bb.put(ReadFromfile("icon18Src.txt", ctx)); break;
					case 19: bb.put(ReadFromfile("icon19Src.txt", ctx)); break;
				}
				bb.position(0);
				GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_LUMINANCE, 32, 32, 0, GLES20.GL_LUMINANCE, GLES20.GL_UNSIGNED_BYTE, bb);
				GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0);
			}
			// Shaders
			mVertexShader = GLES20.glCreateShader(GLES20.GL_VERTEX_SHADER);
			GLES20.glShaderSource(mVertexShader, vertexShaderSrc);
			GLES20.glCompileShader(mVertexShader);
			mFragmentShader = GLES20.glCreateShader(GLES20.GL_FRAGMENT_SHADER);
			GLES20.glShaderSource(mFragmentShader, fragmentShaderSrc);
			GLES20.glCompileShader(mFragmentShader);
			mProgram = GLES20.glCreateProgram();
			GLES20.glAttachShader(mProgram, mVertexShader);
			GLES20.glAttachShader(mProgram, mFragmentShader);
			GLES20.glLinkProgram(mProgram);
			mVertexAttribute = GLES20.glGetAttribLocation(mProgram, "a_VertPosition");
			mTexCoordAttribute = GLES20.glGetAttribLocation(mProgram, "a_TexCoord");
			mTextureUniform = GLES20.glGetUniformLocation(mProgram, "u_TextureUnit");
			// Finalize
			mInitialized = true;
		} else {
			// Textures
			mTextures = new int[20];
			GLES10.glEnable(GLES10.GL_TEXTURE_2D);
			GLES10.glGenTextures(20, mTextures, 0);
			for (int nTex = 0; nTex < 20; nTex++) {
				GLES10.glBindTexture(GLES10.GL_TEXTURE_2D, mTextures[nTex]);
				GLES10.glTexParameterx(GLES10.GL_TEXTURE_2D, GLES10.GL_TEXTURE_MIN_FILTER, GLES10.GL_LINEAR);
				GLES10.glTexParameterx(GLES10.GL_TEXTURE_2D, GLES10.GL_TEXTURE_MAG_FILTER, GLES10.GL_LINEAR);
				GLES10.glTexParameterx(GLES10.GL_TEXTURE_2D, GLES10.GL_TEXTURE_WRAP_S, GLES10.GL_REPEAT);
				GLES10.glTexParameterx(GLES10.GL_TEXTURE_2D, GLES10.GL_TEXTURE_WRAP_T, GLES10.GL_REPEAT);
				GLES10.glTexEnvx(GLES10.GL_TEXTURE_ENV, GLES10.GL_TEXTURE_ENV_MODE, GLES10.GL_MODULATE);
				ByteBuffer bb = ByteBuffer.allocate(32 * 32);
				switch (nTex) {
					case 0: bb.put(ReadFromfile("icon00Src.txt", ctx)); break;
					case 1: bb.put(ReadFromfile("icon01Src.txt", ctx)); break;
					case 2: bb.put(ReadFromfile("icon02Src.txt", ctx)); break;
					case 3: bb.put(ReadFromfile("icon03Src.txt", ctx)); break;
					case 4: bb.put(ReadFromfile("icon04Src.txt", ctx)); break;
					case 5: bb.put(ReadFromfile("icon05Src.txt", ctx)); break;
					case 6: bb.put(ReadFromfile("icon06Src.txt", ctx)); break;
					case 7: bb.put(ReadFromfile("icon07Src.txt", ctx)); break;
					case 8: bb.put(ReadFromfile("icon08Src.txt", ctx)); break;
					case 9: bb.put(ReadFromfile("icon09Src.txt", ctx)); break;
					case 10: bb.put(ReadFromfile("icon10Src.txt", ctx)); break;
					case 11: bb.put(ReadFromfile("icon11Src.txt", ctx)); break;
					case 12: bb.put(ReadFromfile("icon12Src.txt", ctx)); break;
					case 13: bb.put(ReadFromfile("icon13Src.txt", ctx)); break;
					case 14: bb.put(ReadFromfile("icon14Src.txt", ctx)); break;
					case 15: bb.put(ReadFromfile("icon15Src.txt", ctx)); break;
					case 16: bb.put(ReadFromfile("icon16Src.txt", ctx)); break;
					case 17: bb.put(ReadFromfile("icon17Src.txt", ctx)); break;
					case 18: bb.put(ReadFromfile("icon18Src.txt", ctx)); break;
					case 19: bb.put(ReadFromfile("icon19Src.txt", ctx)); break;
				}
				bb.position(0);
				GLES10.glTexImage2D(GLES10.GL_TEXTURE_2D, 0, GLES10.GL_ALPHA, 32, 32, 0, GLES10.GL_ALPHA, GLES10.GL_UNSIGNED_BYTE, bb);
				GLES10.glBindTexture(GLES10.GL_TEXTURE_2D, 0);
			}
			GLES10.glDisable(GLES10.GL_TEXTURE_2D);
			// Finalize
			mInitialized = true;
		}
	}
	
	public void destroy(int GLESVER) {
		if (!mInitialized)
			return;
		if (GLESVER == 2) {
			GLES20.glDeleteProgram(mProgram);
			GLES20.glDeleteShader(mFragmentShader);
			GLES20.glDeleteShader(mVertexShader);
			GLES20.glDeleteTextures(20, mTextures, 0);
		} else {
			GLES10.glDeleteTextures(20, mTextures, 0);
		}
		mInitialized = false;
	}
	
	private void offsetBuf(FloatBuffer to, FloatBuffer from, float x, float y) {
		to.clear();
		for (int n = 0; n < from.capacity(); n += 2) {
			to.put(from.get(n) + x);
			to.put(from.get(n + 1) + y);
		}
		to.position(0);
	}
	
	private void drawTextureAt(int GLESVER, float x, float y, int nTexture,
			                   FloatBuffer vp, FloatBuffer vbb, FloatBuffer tb) {
		offsetBuf(vbb, vp, x, y);
		
		if (GLESVER == 2) {
			GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
			GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mTextures[nTexture]);
			GLES20.glUniform1i(mTextureUniform, 0);
  	    
			GLES20.glVertexAttribPointer(mVertexAttribute, 2, GLES20.GL_FLOAT, false, FLOAT_SZ * 2, vbb);
			GLES20.glEnableVertexAttribArray(mVertexAttribute);
			GLES20.glVertexAttribPointer(mTexCoordAttribute, 2, GLES20.GL_FLOAT, false, FLOAT_SZ * 2, tb);
			GLES20.glEnableVertexAttribArray(mTexCoordAttribute);
			GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4);
			GLES20.glDisableVertexAttribArray(mVertexAttribute);
			GLES20.glDisableVertexAttribArray(mTexCoordAttribute);
  	    
			GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0);
		} else {
			GLES10.glBindTexture(GLES10.GL_TEXTURE_2D, mTextures[nTexture]);
			GLES10.glTexEnvx(GLES10.GL_TEXTURE_ENV, GLES10.GL_TEXTURE_ENV_MODE, GLES10.GL_MODULATE);

			GLES10.glVertexPointer(2, GLES10.GL_FLOAT, FLOAT_SZ * 2, vbb);
			GLES10.glEnableClientState(GLES10.GL_VERTEX_ARRAY);
			GLES10.glTexCoordPointer(2, GLES10.GL_FLOAT, FLOAT_SZ * 2, tb);
			GLES10.glEnableClientState(GLES10.GL_TEXTURE_COORD_ARRAY);
			GLES10.glDrawArrays(GLES10.GL_TRIANGLE_STRIP, 0, 4);
			GLES10.glDisableClientState(GLES10.GL_VERTEX_ARRAY);
			GLES10.glDisableClientState(GLES10.GL_TEXTURE_COORD_ARRAY);

			GLES10.glBindTexture(GLES10.GL_TEXTURE_2D, 0);
		}
	}
	
	public void render(int GLESVER, int winWidthPx, int winHeightPx) {
		if (!mInitialized)
			return;
		if (mTask != -1) {
			Log.i("Toolbar", String.format("Clicked %d button", mTask));
			if (mTask < 7) {
				TeighaDWGJni.viewSetRenderMode(mTask);
				mRenderMode = -1;
			} else if (mTask == 7) {
				mDrawFPS = !mDrawFPS;
			} else if (mTask == 8) {
				TeighaDWGJni.viewRegen();
			}
			mTask = -1;
		}
		{
			final float btnSize = (float)Math.min(winWidthPx, winHeightPx) * BTN_SCALE;
			final float btnSizeX = btnSize / winWidthPx; 
			final float btnSizeY = btnSize / winHeightPx; 

			FloatBuffer vb = ByteBuffer.allocateDirect(FLOAT_SZ * 4 * 2).order(ByteOrder.nativeOrder()).asFloatBuffer();
			vb.put(0.0f); vb.put(0.0f);
			vb.put(btnSizeX); vb.put(0.0f);
			vb.put(0.0f); vb.put(btnSizeY);
			vb.put(btnSizeX); vb.put(btnSizeY);
			vb.position(0);
			FloatBuffer vbb = ByteBuffer.allocateDirect(FLOAT_SZ * 4 * 2).order(ByteOrder.nativeOrder()).asFloatBuffer();

	        FloatBuffer tb = ByteBuffer.allocateDirect(FLOAT_SZ * 4 * 2).order(ByteOrder.nativeOrder()).asFloatBuffer();
			tb.put(0.0f); tb.put(1.0f);
			tb.put(1.0f); tb.put(1.0f);
			tb.put(0.0f); tb.put(0.0f);
			tb.put(1.0f); tb.put(0.0f);
			tb.position(0);

			if (GLESVER == 2) {
				GLES20.glViewport(0, 0, winWidthPx, winHeightPx);
				GLES20.glUseProgram(mProgram);
				GLES20.glEnable(GLES20.GL_BLEND);
				GLES20.glBlendFunc(GLES20.GL_SRC_ALPHA, GLES20.GL_ONE_MINUS_SRC_ALPHA);
			} else {
				GLES10.glViewport(0, 0, winWidthPx, winHeightPx);
				GLES10.glEnable(GLES10.GL_BLEND);
				GLES10.glBlendFunc(GLES10.GL_SRC_ALPHA, GLES10.GL_ONE_MINUS_SRC_ALPHA);
				GLES10.glMatrixMode(GLES10.GL_MODELVIEW);
				GLES10.glLoadIdentity();
				GLES10.glMatrixMode(GLES10.GL_PROJECTION);
				GLES10.glLoadIdentity();
				GLES10.glEnable(GLES10.GL_TEXTURE_2D);
				GLES10.glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			}
      	    
      	    if (mDrawFPS) {
      	      String fps = Float.toString(mFPS);
      	      int nChars = Math.min(fps.length(), 8);
      	      float xBase = -1.0f, yBase = 1.0f - btnSizeY;
      	      for (int nChar = 0; nChar < nChars; nChar++) {
      	    	  char chr = fps.charAt(nChar);
      	    	  int nTex = 10;
      	    	  if (chr >= '0' && chr <= '9')
      	    		  nTex = (int)chr - '0';
      	    	  drawTextureAt(GLESVER, xBase, yBase, nTex, vb, vbb, tb);
      	    	  xBase += btnSizeX;
      	      }
      	      drawTextureAt(GLESVER, xBase, yBase, 11, vb, vbb, tb);
      	    }
      	    
      	    // Render mode
      	    if (mRenderMode == -1) {
      	    	mRenderMode = TeighaDWGJni.viewGetRenderMode();
      	    	mModelSpace = TeighaDWGJni.viewCanRotate();
      	    }
      	    {
      	    	float xBase = 1.0f - btnSizeX * 3, yBase = 1.0f - btnSizeY;
      	    	drawTextureAt(GLESVER, xBase, yBase, 19, vb, vbb, tb);
      	    	drawTextureAt(GLESVER, xBase + btnSizeX, yBase, GLESVER, vb, vbb, tb);
      	    	drawTextureAt(GLESVER, xBase + btnSizeX * 2, yBase, mRenderMode + 12, vb, vbb, tb);
      	    }
      	    
      	    // Toolbar
      	    {
      	    	float xBase = -1.0f, yBase = -1.0f;
      	    	if (mModelSpace) {
      	    		for (int nMode = 0; nMode < 7; nMode++) {
      	    			drawTextureAt(GLESVER, xBase, yBase, 12 + nMode, vb, vbb, tb);
      	    			xBase += btnSizeX;
      	    		}
      	    	}
      	    	drawTextureAt(GLESVER, xBase, yBase, 11, vb, vbb, tb);
      	    	drawTextureAt(GLESVER, xBase + btnSizeX, yBase, 0, vb, vbb, tb);
      	    }
      	    
      	    if (GLESVER == 2) {
      	    	GLES20.glDisable(GLES20.GL_BLEND);
      	    	GLES20.glUseProgram(0);
      	    } else {
				GLES10.glDisable(GLES10.GL_TEXTURE_2D);
      	    	GLES10.glDisable(GLES10.GL_BLEND);
      	    }
		}
	}
    public byte[] ReadFromfile(String fileName, Context context) {
        StringBuilder returnString = new StringBuilder();
        InputStream fIn = null;
        InputStreamReader isr = null;
        BufferedReader input = null;
        try {
            fIn = context.getResources().getAssets()
                    .open(fileName, Context.MODE_WORLD_READABLE);
            input = new BufferedReader(new InputStreamReader(fIn, "UTF-8"), 8192);
            String line = "";
            while ((line = input.readLine()) != null) {
                returnString.append(line);
            }
        } catch (Exception e) {
            e.getMessage();
        } finally {
            try {
                if (isr != null)
                    isr.close();
                if (fIn != null)
                    fIn.close();
                if (input != null)
                    input.close();
            } catch (Exception e2) {
                e2.getMessage();
            }
        }
        String arr[] = returnString.toString().replaceAll("\\s+","").split(",");

        byte[] resByteArr = new byte[arr.length];

        int count = 0;
        for (String val: arr)
            resByteArr[count++] = Byte.parseByte(val);
        return resByteArr;
    }
}
