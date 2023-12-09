#include <iostream>
#include <set>
#include <vector>
#include <math.h>
#include <thread>
#include <mutex>

std::mutex mutex;

bool isPrime(int num) {
    int sqrt = static_cast<int>(std::sqrt(num));

    for (int i = 2; i <= sqrt; i++) {
        if (num % i == 0) return false;
    }

    return true;
}

std::vector<std::pair<int, int>> getPrimeFactorization(int a) {
    std::vector<std::pair<int, int>> factorized;

    int remainder = a;
    int number = 2;
    
    while (remainder != 1) {
        int exponent = 1;
        if (!isPrime(number)) {
            number++;
            continue;
        }

        while (true) {
            int exponentedNum = static_cast<int>(std::pow(number, exponent));
            if (!(remainder % exponentedNum == 0 && exponentedNum <= remainder)) break;
            exponent++;
        }

        exponent--;

        if (!exponent) {
            number++;
            continue;
        }

        factorized.push_back({number, exponent});
        remainder /= (int) std::pow(number, exponent);
        number++;
    }

    // for (const auto& itr : factorized) {
    //     std::cout << "The prime is " << itr.first << " the exponent " << itr.second << std::endl;
    // }

    return factorized;
}

int countPowers(int a, int b) {
    std::set<std::vector<std::pair<int, int>>> powers;

    for (int i = 2; i <= a; i++) {
        std::vector<std::pair<int, int>> factorization = getPrimeFactorization(i);

        for (int j = 2; j <= b; j++) {
            std::vector<std::pair<int, int>> currentFactorization;

            for (int k = 0; k < factorization.size(); k++) {
                currentFactorization.push_back({factorization[k].first, factorization[k].second * j});
            }

            std::unique_lock<std::mutex> lock(mutex);
            powers.insert(currentFactorization);
            lock.unlock();
        }
    }

    return powers.size();
}

void countPowers(int start, int a, int b, std::set<std::vector<std::pair<int, int>>>& powers) {
    std::unique_lock<std::mutex> lock1(mutex);
    std::cout << "The thread " << std::this_thread::get_id() << " has started its execution" << std::endl;
    lock1.unlock();

    for (int i = start; i <= a; i++) {
        std::vector<std::pair<int, int>> factorization = getPrimeFactorization(i);

        for (int j = 2; j <= b; j++) {
            std::vector<std::pair<int, int>> currentFactorization;

            for (int k = 0; k < factorization.size(); k++) {
                currentFactorization.push_back({factorization[k].first, factorization[k].second * j});
            }

            std::unique_lock<std::mutex> lock(mutex);
            powers.insert(currentFactorization);
            lock.unlock();
        }
    }

    std::unique_lock<std::mutex> lock2(mutex);
    std::cout << "The thread " << std::this_thread::get_id() << " has finished its execution" << std::endl;
    lock2.unlock();
}

int multithreadedCount(int a, int b, int numThreads) {
    std::set<std::vector<std::pair<int, int>>> powers;
    std::vector<std::thread> threads;
    int segmentSize = (a + numThreads - 1) / numThreads;

    for (int i = 0; i < numThreads; i++) {
        int start = i * segmentSize + 1;
        int end = (i == numThreads - 1) ? a : (i + 1) * segmentSize;
        if (i == 0) start = 2;

        //std::cout << "The start is " << start << ", the end is " << end << std::endl;
        using CountPowersFuncType = void (*)(int, int, int, std::set<std::vector<std::pair<int, int>>>&);
        CountPowersFuncType countPowersFunc = countPowers;
        threads.push_back(std::thread(countPowersFunc, start, end, b, std::ref(powers)));
    }
 
    for (int i = 0; i < numThreads; i++) { 
        if (threads[i].joinable()) {
            threads[i].join();
        } else {
            std::cout << "Error joining thread " << i << std::endl;
        }
    }

    return powers.size();
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Wrong use of programm" << std::endl;
        std::cout << "main <a(int)> <b(int)>" << std::endl;
        return EXIT_FAILURE;
    }

    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    if (!((a >= 2 && a <= 1000) && (b >= 2 && b <= 1000))) {
        std::cout << "Wrong use of programm" << std::endl;
        std::cout << "2 <= a, b <= 1000" << std::endl;
        return EXIT_FAILURE;
    }

    auto start = std::chrono::high_resolution_clock::now();

    // int count = multithreadedCount(a, b, 6);
    // std::cout << "Multithreaded Pocet se rovna " << count << std::endl;
    std::cout << "Pocet se rovna " << countPowers(a, b) << std::endl;

    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start) / (float) 1e6;
    std::cout << "Time taken: " << duration.count() << " seconds." << std::endl;

    return EXIT_SUCCESS;
}