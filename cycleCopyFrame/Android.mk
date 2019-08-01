LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)


LOCAL_SRC_FILES:= \
	cycleCopyFrame.cpp \
 
LOCAL_CFLAGS += -DGL_GLEXT_PROTOTYPES -DEGL_EGLEXT_PROTOTYPES 



LOCAL_SHARED_LIBRARIES := \
	liblog

LOCAL_C_INCLUDES := \
	$(call include-path-for, corecg graphics) \

LOCAL_MODULE:= cycleCopyFrame
LOCAL_MODULE_TAGS := eng


include $(BUILD_EXECUTABLE)
