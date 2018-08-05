# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(PROJECT_PATH)/lvgl $(PROJECT_PATH)/lvgl/lv_core $(PROJECT_PATH)/lvgl/lv_draw $(PROJECT_PATH)/lvgl/lv_objx $(PROJECT_PATH)/lvgl/lv_hal $(PROJECT_PATH)/lvgl/lv_misc $(PROJECT_PATH)/lvgl/lv_misc/lv_fonts $(PROJECT_PATH)/lvgl/lv_themes $(PROJECT_PATH)
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/lvgl -llvgl
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += lvgl
component-lvgl-build: 
