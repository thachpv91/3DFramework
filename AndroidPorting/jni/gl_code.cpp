/*
 * Copyright (C) 2009 The Android Open Source Project
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

// OpenGL ES 2.0 code

#include <jni.h>
#include "../../InternshipFW/Game.h"
#include "../../InternshipFW/HelloTriangles.h"


static jobject bridge;
static jmethodID android_playSound;
static jmethodID android_playMusic;
static jmethodID android_stopSound;
static jmethodID android_stopMusic;
static JavaVM* jVM;

extern "C" {
    JNIEXPORT void JNICALL Java_com_android_porting_GL2JNILib_init(JNIEnv * env, jobject obj, jstring sdpath, jint width, jint height);
    JNIEXPORT void JNICALL Java_com_android_porting_GL2JNILib_step(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_android_porting_GL2JNILib_onTouch(JNIEnv * env, jobject obj, jint touchType, jint x1, jint y1, jint x2, jint y2);
	JNIEXPORT void JNICALL Java_com_android_porting_GL2JNILib_BridgeJ2C(JNIEnv* env, jobject thiz, jobject obj);

	int JNI_OnLoad(JavaVM* vm, void* reserved)
	{
	    JNIEnv *env;
	    jVM = vm;

	    LOGI("JNI ONLOAD");

	    if((vm)->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK)
	    {
	        return -1;
	    }

	    return JNI_VERSION_1_6;
	}

	// Play Sound
	void PlaySound(int id, int loop)
	{
		JNIEnv *env;
		(jVM)->GetEnv((void**) &env, JNI_VERSION_1_6);
		(env)->CallVoidMethod(bridge, android_playSound, id, loop);
		LOGI("\nPlay Sound from jni!!");
	};

	void PlayMusic(int id, int loop)
	{
		JNIEnv *env;
		(jVM)->GetEnv((void**) &env, JNI_VERSION_1_6);
		(env)->CallVoidMethod(bridge, android_playMusic, id, loop);
		LOGI("\nPlay Music from jni!!");
	}

	void StopSound()
	{
		JNIEnv *env;
		(jVM)->GetEnv((void**) &env, JNI_VERSION_1_6);
		(env)->CallVoidMethod(bridge, android_stopSound);
	}

	void StopMusic()
	{
		JNIEnv *env;
		(jVM)->GetEnv((void**) &env, JNI_VERSION_1_6);
		(env)->CallVoidMethod(bridge, android_stopMusic);
	}
};

JNIEXPORT void JNICALL Java_com_android_porting_GL2JNILib_init(JNIEnv * env, jobject obj, jstring sdpath, jint width, jint height)
{
	Game* game = Game::GetInstance();

	game->Init(env->GetStringUTFChars(sdpath, NULL), width, height);

	game->SetState(new HelloTriangles());

	//PlayMusic(1, 1);
}

JNIEXPORT void JNICALL Java_com_android_porting_GL2JNILib_step(JNIEnv * env, jobject obj)
{
	Game::GetInstance()->Update();
}

JNIEXPORT void JNICALL Java_com_android_porting_GL2JNILib_onTouch(JNIEnv * env, jobject obj, jint touchType, jint x1, jint y1, jint x2, jint y2)
{
	Game::GetInstance()->OnPointer(touchType, x1, y1, x2, y2);
}

JNIEXPORT void JNICALL Java_com_android_porting_GL2JNILib_BridgeJ2C( JNIEnv* env, jobject thiz, jobject obj )
{
    bridge = obj;
    jclass glViewClass;

    (jVM)->GetEnv((void**) &env, JNI_VERSION_1_6);

    bridge = (jobject)(env)->NewGlobalRef( obj);
    glViewClass = (env)->GetObjectClass( bridge);

    android_playSound = (env)->GetMethodID(glViewClass, "playSound", "(II)V");
    android_playMusic = (env)->GetMethodID(glViewClass, "playMusic", "(II)V");
    android_stopSound = (env)->GetMethodID(glViewClass, "stopSound", "()V");
    android_stopMusic = (env)->GetMethodID(glViewClass, "stopMusic", "()V");

	LOGI("End C call java init");
}
