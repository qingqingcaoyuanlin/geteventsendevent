LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS) 
LOCAL_LDLIBS := -llog
#CFLAGS += -fvisibility=default -fPIE
#LDFLAGS += -rdynamic -fPIE -pie
# give module name
LOCAL_MODULE    := sendevent  
# list your C files to compile
LOCAL_SRC_FILES := sendevent.c
# this option will build executables instead of building library for android application.
include $(BUILD_EXECUTABLE)
