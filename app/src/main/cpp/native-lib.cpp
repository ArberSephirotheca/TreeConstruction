//#include "app_params.hpp"
//#include "naive_pipe.hpp"
//#include <unistd.h>
//#include <vulkan/vulkan.hpp>
//#include "volk.h"
#include <jni.h>
//#include <chrono>
#include <string>

#define BUFFER_ELEMENTS  50000


extern "C" JNIEXPORT int JNICALL
Java_com_example_treeconstruction_MainActivity_androidMain(
        JNIEnv* env,
        jobject /* this */) {
//    if (volkInitialize() != VK_SUCCESS) {
//        std::cerr << "Failed to initialize volk!" << std::endl;
//        return EXIT_FAILURE;
//    }
    int n_blocks = 1;
//    AppParams app_params;
//    app_params.n = BUFFER_ELEMENTS;
//    app_params.min_coord = 0.0f;
//    app_params.max_coord = 1.0f;
//    app_params.seed = 114514;
//    app_params.n_threads = 4;
//    app_params.n_blocks = n_blocks;

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
    return 0;
}