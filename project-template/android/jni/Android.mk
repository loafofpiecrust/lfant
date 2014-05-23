
LOCAL_PATH := bin/lib
include $(CLEAR_VARS)

LOCAL_MODULE := libz
LOCAL_SRC_FILES := libz.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libjpeg
LOCAL_SRC_FILES := libjpeg.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libpng
LOCAL_SRC_FILES := libpng.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libassimp
LOCAL_SRC_FILES := libassimp.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libbullet
LOCAL_SRC_FILES := libbullet.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := liblfant
LOCAL_SRC_FILES := liblfant.so
include $(PREBUILT_SHARED_LIBRARY)

LOCAL_PATH := bin
include $(CLEAR_VARS)

LOCAL_MODULE := libgalaga
LOCAL_SRC_FILES := libgalaga.so
include $(PREBUILT_SHARED_LIBRARY)

LOCAL_ARM_MODE := arm
