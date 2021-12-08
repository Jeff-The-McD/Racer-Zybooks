
#include <iostream>
#include<thread>
#include <vector>
#include <random>
#include <functional>
bool thread_running = true;
std::uniform_int_distribution<int> time_dist(1,5);
std::uniform_int_distribution<int> slot_dist(1,10);
std::mt19937 random_number_engine;
auto time_roller = std::bind(time_dist, random_number_engine);
auto slot_roller = std::bind(slot_dist,random_number_engine);

void producer(std::vector<int> buffer_to_work,int n){
    int i = 0;
    int next_in = 0;
    while(thread_running){
        i++;
        int k1 = slot_roller();
        
        for(int i = 0; i<k1; ++i){
            buffer_to_work[next_in + (k1%n)] = 1;
        }
        next_in = next_in + (k1%n);
        std::cout<< "Producer next_in:"<< next_in<< std::endl;

        int t1 = time_roller();
        std::this_thread::sleep_for(std::chrono::seconds(t1));
        if(i>n || thread_running ==false) return;

    }
}

void consumer(std::vector<int> buffer_to_work ,int n){
    int i = 0;
    int next_out = 0;
    while (thread_running){
        i++;

        int t2 = time_roller();
        std::this_thread::sleep_for(std::chrono::seconds(t2));
        int k2 = slot_roller();
        

        for(int i = 0;i<k2;++i){
            int data = buffer_to_work[next_out+(k2%n)];
            if(data>1){

                std::cout<<"Given a buffer of size "<< n <<" Consumer failed to catch up.\n";
                
                return;
            }
            buffer_to_work[next_out+(k2%n)] = 0;
            next_out = next_out + (k2%n);
        }
        std::cout<< "Consumer next_out:"<< next_out<< std::endl;
        if(i>n) return;
    }
}
int main(int argc, char** argv){
    int n = atoi(argv[1]);
    std::cout<< n << std::endl;
    std::vector<int> buffer(n,0); 

    std::thread consumer_thread(consumer,ref(buffer),n);
    std::thread producer_thread(producer,ref(buffer),n);

    consumer_thread.join();
    thread_running = false;
    producer_thread.join();
    std::cout<<" Done!";

    return EXIT_SUCCESS;
}