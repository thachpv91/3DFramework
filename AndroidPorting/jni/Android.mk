# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)
CPP_PATH   := ../../InternshipFW

include $(CLEAR_VARS)

LOCAL_MODULE    := libgl2jni
LOCAL_CFLAGS    := -Werror
LOCAL_SRC_FILES :=	gl_code.cpp \
					$(CPP_PATH)/Camera.cpp \
					$(CPP_PATH)/Game.cpp \
					$(CPP_PATH)/GameObject.cpp \
					$(CPP_PATH)/HelloTriangles.cpp \
					$(CPP_PATH)/InputManager.cpp \
					$(CPP_PATH)/Light.cpp \
					$(CPP_PATH)/Matrix.cpp \
					$(CPP_PATH)/Model.cpp \
					$(CPP_PATH)/Object.cpp \
					$(CPP_PATH)/ResourceManager.cpp \
					$(CPP_PATH)/SceneManager.cpp \
					$(CPP_PATH)/Shader.cpp \
					$(CPP_PATH)/Texture.cpp \
					$(CPP_PATH)/TGA.cpp \
					$(CPP_PATH)/Timer.cpp \
					$(CPP_PATH)/Vector2.cpp \
					$(CPP_PATH)/Vector3.cpp \
					$(CPP_PATH)/Vector4.cpp \
					$(CPP_PATH)/MTL.cpp \
					$(CPP_PATH)/PNG.cpp \

LOCAL_LDLIBS    := -llog -lGLESv2
LOCAL_SHARED_LIBRARIES := stlport_shared

include $(BUILD_SHARED_LIBRARY)
