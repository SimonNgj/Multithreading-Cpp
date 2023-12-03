/* In this example, we simulate an image represented as a matrix of pixels. 
The image processing functions (applyBlur and applySharpen) are designed to work on specific segments of the image concurrently. 
We utilize multiple threads to apply these filters simultaneously, dividing the image into segments for parallel processing.

A mutex (myMutex) is employed to ensure thread safety when accessing shared resources, in this case, portions of the image matrix.
*/
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>

// Define image processing functions
void applyBlur(std::vector<int>& image, int startX, int endX, std::mutex& myMutex) {
    // Apply blur filter to the specified portion of the image
    std::lock_guard<std::mutex> lock(myMutex);
    // Code for blur filter
    for (int i = startX; i < endX; i++)
    {
        image[i] *= 3;
    }
}

void applySharpen(std::vector<int>& image, int startX, int endX, std::mutex& myMutex) {
    // Apply sharpen filter to the specified portion of the image
    std::lock_guard<std::mutex> lock(myMutex);
    // Code for sharpen filter
    for (int i = startX; i < endX; i++)
    {
        image[i] += 100;
    }
}

int main() {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    // Load the image matrix
    std::vector<int> image = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; 

    std::cout <<std::endl<< "Input:" << std::endl;
    for (int i = 0; i < image.size(); i++)
    {
        std::cout << i << ". " << image[i] << std::endl;
    }

    // Divide the image into segments for concurrent processing
    int segments = std::thread::hardware_concurrency();
    int segmentSize = image.size() / segments;

    // Mutex for thread synchronization
    std::mutex myMutex;

    // Create threads for concurrent image processing
    std::vector<std::thread> threads;
    for (int i = 0; i < segments; i++) {
        int startX = i * segmentSize;
        int endX = (i == segments - 1) ? image.size() : startX + segmentSize;
        threads.emplace_back(applyBlur, std::ref(image), startX, endX, std::ref(myMutex));
        threads.emplace_back(applySharpen, std::ref(image), startX, endX, std::ref(myMutex));
    }

    // Join threads to wait for their completion
    for (auto& thread : threads) {
        thread.join();
    }

    // Display or save the processed image

    std::cout << "Output:" << std::endl;
    for (int i = 0; i < image.size(); i++)
    {
        std::cout <<i<<". "<< image[i] << std::endl;
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << std::endl<<"Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;

    return 0;
}
