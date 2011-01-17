#
# Copyright (C) 2011 NXP Software
# Copyright (C) 2011 The Android Open Source Project
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

LOCAL_PATH:= $(call my-dir)

#
# libvideoeditorplayer
#

include $(CLEAR_VARS)

LOCAL_MODULE:= libvideoeditorplayer

LOCAL_SRC_FILES:=          \
    VideoEditorTools.cpp \
    VideoEditorPlayer.cpp \
    PreviewPlayer.cpp \
    VideoEditorAudioPlayer.cpp \
    VideoEditorPreviewController.cpp \
    VideoEditorSRC.cpp \
    DummyAudioSource.cpp \
    DummyVideoSource.cpp \
    VideoEditorBGAudioProcessing.cpp \
    PreviewRenderer.cpp

LOCAL_MODULE_TAGS := development

LOCAL_STATIC_LIBRARIES := \
    libvideoeditor_osal \
    libstagefright_color_conversion



LOCAL_SHARED_LIBRARIES := \
    libbinder          \
    libutils           \
    libcutils          \
    libmedia           \
    libstagefright  \
    libstagefright_omx  \
    libstagefright_foundation \
    libsurfaceflinger_client \
    libaudioflinger \
    libui


LOCAL_C_INCLUDES += \
    $(TOP)/frameworks/base/core/jni \
    $(TOP)/frameworks/base/include \
    $(TOP)/frameworks/base/include/media \
    $(TOP)/frameworks/base/media/libmediaplayerservice \
    $(TOP)/frameworks/base/media/libstagefright \
    $(TOP)/frameworks/base/media/libstagefright/include \
    $(TOP)/frameworks/base/media/libstagefright/rtsp \
    $(JNI_H_INCLUDE) \
    $(call include-path-for, corecg graphics) \
    $(TOP)/external/opencore/extern_libs_v2/khronos/openmax/include \
    $(TOP)/external/opencore/android \
    $(TOP)/vendor/qcom/proprietary/qdsp6/mm-core/omxcore/inc \
    $(TOP)/frameworks/media/libvideoeditor/osal/inc \
    $(TOP)/frameworks/media/libvideoeditor/vss/common/inc \
    $(TOP)/frameworks/media/libvideoeditor/vss/mcs/inc \
    $(TOP)/frameworks/media/libvideoeditor/vss/inc \
    $(TOP)/frameworks/media/libvideoeditor/vss/stagefrightshells/inc \
    $(TOP)/frameworks/media/libvideoeditor/lvpp \
    $(TOP)/frameworks/base/media/jni/mediaeditor \
    $(TOP)/frameworks/base/services/audioflinger


ifeq ($(TARGET_SIMULATOR),true)
else
    LOCAL_SHARED_LIBRARIES += libdl
endif

# All of the shared libraries we link against.
LOCAL_LDLIBS := \
    -lpthread -ldl

LOCAL_CFLAGS += -Wno-multichar \
     -DM4_ENABLE_RENDERINGMODE \
    -DUSE_STAGEFRIGHT_CODECS \
    -DUSE_STAGEFRIGHT_AUDIODEC \
    -DUSE_STAGEFRIGHT_VIDEODEC \
    -DUSE_STAGEFRIGHT_AUDIOENC \
    -DUSE_STAGEFRIGHT_VIDEOENC \
    -DUSE_STAGEFRIGHT_READERS \
    -DUSE_STAGEFRIGHT_3GPP_READER


# Don't prelink this library.  For more efficient code, you may want
# to add this library to the prelink map and set this to true.
LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)

#include $(call all-makefiles-under,$(LOCAL_PATH))