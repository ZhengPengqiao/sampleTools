LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_CLANG := true
LOCAL_C_INCLUDES:=
LOCAL_SRC_FILES:= main.c adpcm.c
LOCAL_MODULE := adpcm
LOCAL_SHARED_LIBRARIES:=
LOCAL_MODULE_TAGS := debug
include $(BUILD_EXECUTABLE)