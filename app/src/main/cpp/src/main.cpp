#include "app_params.hpp"
#include "naive_pipe.hpp"
#include <unistd.h>
//#include <vulkan/vulkan.hpp>
#include "volk.h"
#include <jni.h>
#include <chrono>
#include <string>
#include <android/log.h>

#define BUFFER_ELEMENTS  50000
static jobject gAssetManagerGlobalRef = nullptr;
static AAssetManager* gAAssetManager = nullptr;

// Global file pointer
FILE *file = nullptr;

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_treeconstruction_MainActivity_getInternalStoragePath(JNIEnv *env, jobject obj) {
    // Assuming your package name is 'com.example.yourapp'
    // Get the context class and methods
    jclass contextClass = env->FindClass("android/content/Context");
    jmethodID getFilesDir = env->GetMethodID(contextClass, "getFilesDir", "()Ljava/io/File;");
    jobject file = env->CallObjectMethod(obj, getFilesDir);
    jclass fileClass = env->FindClass("java/io/File");
    jmethodID getAbsolutePath = env->GetMethodID(fileClass, "getAbsolutePath", "()Ljava/lang/String;");
    jstring path = (jstring) env->CallObjectMethod(file, getAbsolutePath);
    return path;
}


extern "C" JNIEXPORT int JNICALL
Java_com_example_treeconstruction_MainActivity_androidMain(
        JNIEnv* env,
        jobject /* this */,
        jobject assetManager) {
    // Create a global reference to keep the AssetManager alive
    if (gAssetManagerGlobalRef == nullptr) {
        gAssetManagerGlobalRef = env->NewGlobalRef(assetManager);
        gAAssetManager = AAssetManager_fromJava(env, gAssetManagerGlobalRef);
    }
    if (!gAAssetManager) {
        __android_log_print(ANDROID_LOG_ERROR, "JNI", "Could not retrieve asset manager");
        return -1;
    }
//    __android_log_print(ANDROID_LOG_DEBUG, "Main", "Start to load shader");
//    AAssetDir* dir = AAssetManager_openDir(mgr, "shaders");
//    auto file_name = AAssetDir_getNextFileName(dir);
//    __android_log_print(ANDROID_LOG_DEBUG, "Main", "got file name %s", file_name);
//    AAssetDir_close(dir);

    if (volkInitialize() != VK_SUCCESS) {
        __android_log_print(ANDROID_LOG_ERROR, "Volk", "Failed to initialize volk!");
        return EXIT_FAILURE;
    }else{
        __android_log_print(ANDROID_LOG_DEBUG, "VOlk", "Successfully initialize volk!");
    }
    int n_blocks = 1;
    AppParams app_params;
    app_params.n = BUFFER_ELEMENTS;
    app_params.min_coord = 0.0f;
    app_params.max_coord = 1.0f;
    app_params.seed = 114514;
    app_params.n_threads = 4;
    app_params.n_blocks = n_blocks;

    Pipe pipe = Pipe(app_params, gAAssetManager);
    __android_log_print(ANDROID_LOG_DEBUG, "Pipeline", "Piepeline created");
    pipe.allocate();
    __android_log_print(ANDROID_LOG_DEBUG, "Pipeline", "Memory Allocated");

    pipe.init(n_blocks, 0);
//    std::cout <<"morton"<<std::endl;
    pipe.morton(n_blocks, 0);

    //pipe.radix_sort(n_blocks, 0);
     pipe.radix_sort_alt(n_blocks, 0);

//     pipe.unique(n_blocks, 0);
//    pipe.radix_sort_alt(n_blocks, 0);

    // pipe.radix_tree(n_blocks, 0);

    // pipe.edge_count(n_blocks, 0);

    // pipe.prefix_sum(n_blocks, 0);

    // pipe.octree(n_blocks, 0);
    return EXIT_SUCCESS;
}

//void android_main(const int argc, const char* argv[]){
//  if (volkInitialize() != VK_SUCCESS) {
//    std::cerr << "Failed to initialize volk!" << std::endl;
//  }
//    int n_blocks = 1;
//
//    if (argc > 1){
//        n_blocks = std::stoi(argv[1]);
//    }
//    AppParams app_params;
//    app_params.n = BUFFER_ELEMENTS;
//    app_params.min_coord = 0.0f;
//    app_params.max_coord = 1.0f;
//    app_params.seed = 114514;
//    app_params.n_threads = 4;
//    app_params.n_blocks = n_blocks;
//
//
//    Pipe pipe = Pipe(app_params);
//    pipe.allocate();
//
//    pipe.init(n_blocks, 0);
//
//    pipe.morton(n_blocks, 0);
//
//    //pipe.radix_sort(n_blocks, 0);
//    // pipe.radix_sort_alt(n_blocks, 0);
//
//    // pipe.unique(n_blocks, 0);
//    pipe.radix_sort_alt(n_blocks, 0);
//
//    // pipe.radix_tree(n_blocks, 0);
//
//    // pipe.edge_count(n_blocks, 0);
//
//    // pipe.prefix_sum(n_blocks, 0);
//
//    // pipe.octree(n_blocks, 0);
    
//}

extern "C" JNIEXPORT jobject JNICALL
Java_com_example_treeconstruction_MainActivity_nativeShutdown(JNIEnv* env, jobject instance) {
    if (gAssetManagerGlobalRef != nullptr) {
        env->DeleteGlobalRef(gAssetManagerGlobalRef);
        gAssetManagerGlobalRef = nullptr;
        gAAssetManager = nullptr;
    }
}


// Function to get internal storage path from Java
extern "C"
JNIEXPORT void JNICALL
Java_com_example_treeconstruction_MainActivity_openFileAndRedirectStdout(JNIEnv *env, jobject obj) {
    if (file != nullptr) {
        // File is already open, no need to open it again
    }

    // Get the class and method ID
    jclass clazz = env->GetObjectClass(obj);
    jmethodID getPathMethod = env->GetMethodID(clazz, "getInternalStoragePath", "()Ljava/lang/String;");
    if (clazz == nullptr) {
        __android_log_print(ANDROID_LOG_ERROR, "JNI", "Failed to get class reference");
    }

    // Call the method and get the path as a jstring
    jstring path = (jstring) env->CallObjectMethod(obj, getPathMethod);
    const char *internalPath = env->GetStringUTFChars(path, 0);

    // Construct the full path for the output file
    std::string filePath = std::string(internalPath) + "/android.txt";

    // Redirect stdout to the file
    file = freopen(filePath.c_str(), "w", stdout);
    if (file == nullptr) {
        __android_log_print(ANDROID_LOG_ERROR , "JNI", "Failed to redirect stdout to file: %s", filePath.c_str());
        perror("freopen");
        exit(EXIT_FAILURE);
    }

    // Release the JNI string
    env->ReleaseStringUTFChars(path, internalPath);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_treeconstruction_MainActivity_closeFile(JNIEnv *env, jobject obj) {
    if (file != nullptr) {
        // Close the file
        fclose(file);
        file = nullptr;
    }
}
