// 引入必要的头文件
#include <iostream> // 用于输入输出流
#include <chrono> // 用于高精度时间测量
#include <cstdlib> // 包含一些通用的工具函数，如随机数生成
#include <pthread.h> // 用于多线程编程
#include <time.h> // 用于时间处理函数
#include "SkipList.h" // 引入自定义的跳表实现

// 定义宏常量
#define NUM_THREADS 2 // 线程数量
#define TEST_COUNT 279000 // 测试用的数据量大小
SkipList<int, std::string> skipList(18); // 创建一个最大层级为18的跳表实例

// 插入元素的线程函数
void *insertElement(void* threadid) {
    long tid; // 线程ID
    tid = (long)threadid; // 将void*类型的线程ID转换为long型
    std::cout << tid << std::endl; // 输出线程ID
    int tmp = TEST_COUNT/NUM_THREADS; // 计算每个线程应该插入的元素数量
    // 循环插入元素
    for (int i=tid*tmp, count=0; count<tmp; i++) {
        count++;
        skipList.insert_element(rand() % TEST_COUNT, "a"); // 随机生成一个键，并插入带有"a"的元素
    }
    pthread_exit(NULL); // 退出线程
}

// 检索元素的线程函数
void *getElement(void* threadid) {
    long tid; // 线程ID
    tid = (long)threadid; // 将void*类型的线程ID转换为long型
    std::cout << tid << std::endl; // 输出线程ID
    int tmp = TEST_COUNT/NUM_THREADS; // 计算每个线程应该检索的元素数量
    // 循环检索元素
    for (int i=tid*tmp, count=0; count<tmp; i++) {
        count++;
        skipList.search_element(rand() % TEST_COUNT); // 随机生成一个键，并尝试检索
    }
    pthread_exit(NULL); // 退出线程
}

int main() {
    srand(time(NULL)); // 初始化随机数生成器
    {
        pthread_t threads[NUM_THREADS]; // 定义线程数组
        int rc; // 用于接收pthread_create的返回值
        int i; // 循环计数器

        auto start = std::chrono::high_resolution_clock::now(); // 开始计时

        // 创建插入元素的线程
        for( i = 0; i < NUM_THREADS; i++ ) {
            std::cout << "main() : creating thread, " << i << std::endl;
            rc = pthread_create(&threads[i], NULL, insertElement, (void *)i); // 创建线程

            if (rc) {
                std::cout << "Error:unable to create thread," << rc << std::endl;
                exit(-1); // 如果线程创建失败，退出程序
            }
        }

        void *ret; // 用于接收pthread_join的返回值
        // 等待所有插入线程完成
        for( i = 0; i < NUM_THREADS; i++ ) {
            if (pthread_join(threads[i], &ret) != 0 )  {
                perror("pthread_create() error");
                exit(3); // 如果线程等待失败，退出程序
            }
        }
        auto finish = std::chrono::high_resolution_clock::now(); // 结束计时
        std::chrono::duration<double> elapsed = finish - start; // 计算耗时
        std::cout << "insert elapsed:" << elapsed.count() << std::endl; // 输出插入操作耗时
    }

    // skipList.display_list();

    // {
    //     pthread_t threads[NUM_THREADS];
    //     int rc;
    //     int i;
    //     auto start = std::chrono::high_resolution_clock::now();

    //     for( i = 0; i < NUM_THREADS; i++ ) {
    //         std::cout << "main() : creating thread, " << i << std::endl;
    //         rc = pthread_create(&threads[i], NULL, getElement, (void *)i);

    //         if (rc) {
    //             std::cout << "Error:unable to create thread," << rc << std::endl;
    //             exit(-1);
    //         }
    //     }

    //     void *ret;
    //     for( i = 0; i < NUM_THREADS; i++ ) {
    //         if (pthread_join(threads[i], &ret) !=0 )  {
    //             perror("pthread_create() error"); 
    //             exit(3);
    //         }
    //     }

    //     auto finish = std::chrono::high_resolution_clock::now(); 
    //     std::chrono::duration<double> elapsed = finish - start;
    //     std::cout << "get elapsed:" << elapsed.count() << std::endl;
    // }

    pthread_exit(NULL); // 主线程退出
    return 0;
}