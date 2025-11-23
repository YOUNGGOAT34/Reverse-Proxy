#ifndef THREADPOOL_H
#define THREADPOOL_H
#include "definitions.hpp"

class THREADPOOL{
      private:
            std::mutex mtx;
            std::condition_variable cv;
            std::vector<std::thread> worker_threads;
            std::queue<std::function<void()>> tasks;
            bool stopping=false;

            void start_threads(i32 number_of_threads);
            void stop();


      public:
         
         THREADPOOL(i32 num_of_threads);
         ~THREADPOOL();

         template<typename F,typename ...Args>
         void enqueue(F&& f,Args&& ...args){
                 auto func=[f=std::forward<F>(f),...args=std::forward<Args>(args)](){
                           f(args...);
                 };

                 {
                       std::unique_lock<std::mutex> loc(mtx);
                       tasks.push(func);
                 }

                 cv.notify_one();

         }
         

};

#endif