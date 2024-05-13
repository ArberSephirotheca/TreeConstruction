//#include "vulkan/vulkan.h"
#include "volk.h"
#include <math.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <fstream>
#include <assert.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <algorithm>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/native_activity.h>
#include <android/log.h>
#include <jni.h>

#define VK_FLAGS_NONE 0

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif


namespace tools{


		VkShaderModule loadShader(const char *fileName, VkDevice device, AAssetManager* assetManager)
		{
//            __android_log_print(ANDROID_LOG_DEBUG, "Main", "Start to load shader");
//            AAssetDir* dir = AAssetManager_openDir(assetManager, "shaders");
//            auto file_name = AAssetDir_getNextFileName(dir);
//            __android_log_print(ANDROID_LOG_DEBUG, "Main", "got file name %s", file_name);
//            AAssetDir_close(dir);
            AAsset* asset = AAssetManager_open(assetManager, fileName, AASSET_MODE_BUFFER);
            if (asset != nullptr) {
                __android_log_print(ANDROID_LOG_DEBUG, "JNI", "Successfully open the shader file %s", fileName);
                size_t size = AAsset_getLength(asset);
                char* shaderCode = new char[size];
                AAsset_read(asset, shaderCode, size);
                AAsset_close(asset);

                VkShaderModule shaderModule;
                VkShaderModuleCreateInfo moduleCreateInfo{};
                moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
                moduleCreateInfo.codeSize = size;
                moduleCreateInfo.pCode = (uint32_t*)shaderCode;

                if (vkCreateShaderModule(device, &moduleCreateInfo, nullptr, &shaderModule) != VK_SUCCESS) {
                    __android_log_print(ANDROID_LOG_ERROR, "Vulkan", "Fail to create shader module");
                }else{
                    __android_log_print(ANDROID_LOG_DEBUG, "Vulkan", "Successfully to create shader module");
                }

                delete[] shaderCode;
                return shaderModule;
            } else {
                __android_log_print(ANDROID_LOG_ERROR, "JNI", "Could not open the shader file");
                return VK_NULL_HANDLE;
            }
		}
}

bool checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

for (const char* layerName : validationLayers) {
    bool layerFound = false;

    for (const auto& layerProperties : availableLayers) {
		printf("Layer: %s\n", layerProperties.layerName);
        if (strcmp(layerName, layerProperties.layerName) == 0) {
            layerFound = true;
            break;
        }
    }

    if (!layerFound) {
        return false;
    }
}

return true;
}
