/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.porting;

import java.io.IOException;

import com.example.androidporting.R;

import android.content.Context;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.SoundPool;


// Wrapper for native library

public class GL2JNILib {
	private static SoundPool soundPlayer;
	private static int sound_1;
	private static int sound_2;
	private static int sound_3;
	private static MediaPlayer musicPlayer = null;
	public static Context sound_context;
	private static GL2JNILib _instance = null;
	public static GL2JNILib Instance()
	{
		if (_instance == null)
			_instance = new GL2JNILib();
		return _instance;
	}
	
	static {
		System.loadLibrary("stlport_shared");
		System.loadLibrary("gl2jni");
	}
	
	public void initSound()
	{
		soundPlayer = new SoundPool(10, AudioManager.STREAM_MUSIC, 0);
		sound_1 = soundPlayer.load(sound_context, R.raw.sd_cannonfire, 1);
		sound_2 = soundPlayer.load(sound_context, R.raw.sd_cannonturn, 1);
		sound_3 = soundPlayer.load(sound_context, R.raw.sd_warning, 1);
	}
	 
	public void playSound(int id, int loop)
	{
		int soundID = 0;
		switch (id)
		{
			case 1: soundID = sound_1; break;
			case 2: soundID = sound_2; break;
			case 3: soundID = sound_3; break;
		}
		soundPlayer.play(soundID, 0.5f, 0.5f, 0, 0, 1.5f);
	}
    
	public void stopSound()
	{
		
	}
    
	public void playMusic(int id, int loop)
	{
		if (musicPlayer != null) 
		{
			try
			{
				if (musicPlayer.isPlaying())
					musicPlayer.stop();
				musicPlayer.release();
			} catch (Exception e) {
			}
		}
		int soundID = 0;
		switch (id)
		{
			case 1: soundID = R.raw.ms_menu; break; // menu
			case 2: soundID = R.raw.ms_battle; break; // battle
			case 3: soundID = R.raw.ms_gameover; break; // gameover
		}
		if (soundID != 0)
			musicPlayer = MediaPlayer.create(sound_context, soundID);
		try {
			musicPlayer.prepare();
		} catch (IllegalStateException e) {
		} catch (IOException e) {
		}
		if (loop == 1)
			musicPlayer.setLooping(true);
		musicPlayer.start();
	}
    
	public void stopMusic()
	{
		if (musicPlayer != null)
		{
			try
			{
				if (musicPlayer.isPlaying())
					musicPlayer.stop();
				musicPlayer.release();
			} catch (Exception e) {
			}
		}
	}

    /**
     * @param width the current view width
     * @param height the current view height
     */
	public static native void BridgeJ2C(GL2JNILib obj);
    public static native void init(String sdpath, int width, int height);
    public static native void step();
    public static native void onTouch(int touchType, int x1, int y1, int x2, int y2);
}
