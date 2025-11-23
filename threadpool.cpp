
#include "threadpool.hpp"


THREADPOOL::THREADPOOL(i32 number_of_threads){
       start_threads(number_of_threads);
}

THREADPOOL::~THREADPOOL(){
     stop();
}


void THREADPOOL::start_threads(i32 number_of_threads){
      for(i32 i=0;i<number_of_threads;++i){
            worker_threads.emplace_back(
                 [this](){
                     while(true){
                       std::function<void()> task;

                       {
                     
                        std::unique_lock<std::mutex> lock(mtx);

                        cv.wait(lock,[this](){
                              return stopping || !tasks.empty();
                        });

                        if(tasks.empty() && stopping){
                            return;
                        }

                        task=tasks.front();
                        tasks.pop();

                       }


                        task();

                     }
                 });
      }
}

void THREADPOOL::stop(){
     {
      std::unique_lock<std::mutex> lock(mtx);
      stopping=true;
     }

     cv.notify_all();

     for(auto &t:worker_threads){
        if(t.joinable()){
          t.join();
        }
     }
}