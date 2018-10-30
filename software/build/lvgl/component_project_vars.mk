# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(PROJECT_PATH)/KairOS/libs/lvgl $(PROJECT_PATH)/KairOS/libs/lvgl/lv_core $(PROJECT_PATH)/KairOS/libs/lvgl/lv_draw $(PROJECT_PATH)/KairOS/libs/lvgl/lv_objx $(PROJECT_PATH)/KairOS/libs/lvgl/lv_hal $(PROJECT_PATH)/KairOS/libs/lvgl/lv_misc $(PROJECT_PATH)/KairOS/libs/lvgl/lv_misc/lv_fonts $(PROJECT_PATH)/KairOS/libs/lvgl/lv_themes $(PROJECT_PATH)/KairOS/libs
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/lvgl -llvgl
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += lvgl
component-lvgl-build: 
