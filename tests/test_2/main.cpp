//
//  main2.cpp
//  test_SCUD
//
//  Created by Boris Vigman on 08/07/2017.
//  Copyright © 2017-2019 Boris Vigman. All rights reserved.
//


#include "scud.h"
using namespace SCUD;

#include <iostream>
#include <thread>
#include <chrono>
#include "vector"

template<typename TSchedulable,typename Tid> class Queue1:public LinkableQueue<int,void*>{
    
};
template<typename TSchedulable,typename Tid> class Dropper1:public LinkableDropper<int,void*>{
public:
    
    void processOnPush(TSchedulable sch, long long schedulingParam){
        int x=0;
    }
    void processOnPull(TSchedulable sch, long long schedulingParam){
        int x=0;
    }
    void processOnPullPush(TSchedulable sch, long long schedulingParam){
        int x=0;
    }
};
class Source{
    char priority;
    float weight;
    int label;
    long long delay;
    Queue1<int,void*> queue0;
    LinkableQueue<int,void*> queue1;
    Dropper1<int,void*> drop0;
    LinkableDropper<int,void*> drop1;
    LinkablePass<int, void*> pass1,pass2;
    //LinkablePass<int,void*> pass;
    void run(){
        SCUD_RC r0;
        drop1.unlink(&r0);
        drop0.setPriority(7);
        drop1.setPriority(priority);
        SCUD_RC rc= queue0.setPriority(priority);std::cout<<SCHelper::convertReturnCodeToString(rc)<<std::endl;
        rc=queue0.setWeight(weight);std::cout<<SCHelper::convertReturnCodeToString(rc)<<std::endl;
        rc=queue0.setLowThreshold(1);std::cout<<SCHelper::convertReturnCodeToString(rc)<<std::endl;
        rc=queue0.setHighThreshold(20);std::cout<<SCHelper::convertReturnCodeToString(rc)<<std::endl;
        rc=queue1.setDRRQuantum(0);
        rc=queue1.setDRRQuantum(50);
        rc=queue1.setWeight(5);std::cout<<SCHelper::convertReturnCodeToString(rc)<<std::endl;
        rc=queue1.setLowThreshold(1);std::cout<<SCHelper::convertReturnCodeToString(rc)<<std::endl;
        rc=queue1.setHighThreshold(30);std::cout<<SCHelper::convertReturnCodeToString(rc)<<std::endl;
        rc= queue1.setPriority(priority);std::cout<<SCHelper::convertReturnCodeToString(rc)<<std::endl;
        int count=10;
        
        
        //while(count>0){
        SCUD_RC status=SCUD_RC_OK;
        //queue.unlink(&status);
        //std::cout<<"loop: source"<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        queue1.linkSuccessor(&drop1);
        queue0.linkSuccessor(&drop0);
        
        drop0.linkSuccessor(&queue0);
        //status=next->linkPredecessor(&drop1);
        
        //}
        while(1){
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            
            //buildCompound();
            auto start= std::chrono::high_resolution_clock::now() ;
            drop0.push(label, label*10);
            auto end= std::chrono::high_resolution_clock::now() ;
            auto diff = end - start;
            //std::cout<<"Push delay="<<std::chrono::duration <double, std::micro>(diff).count()<<std::endl;
            //std::cout<<"Queue1 ["<<label<<"] has "<<queue1.size()<<" objects"<<std::endl;
             start= std::chrono::high_resolution_clock::now() ;
            pass1.pullAndPush();
             end= std::chrono::high_resolution_clock::now() ;
             diff = end - start;
            //std::cout<<"Pull/Push delay="<<std::chrono::duration <double, std::micro>(diff).count()<<std::endl;
            //std::cout<<"Queue0 ["<<label<<"] has "<<queue0.size()<<" objects;"<<"Queue1 ["<<label<<"] has "<<queue1.size()<<" objects"<<std::endl;
            //breakCompound();
            /*
            count--;
            if(count<0){
                count=100;
                std::this_thread::sleep_for(std::chrono::milliseconds(delay*100));
            }
             */
        }
    }
public:
    Source(){weight=-1;priority=-1;label=-1;delay=100;}
    Source(int lbl, char p,float w,long long delayMilli){
        priority=p;
        weight=w;
        label=lbl;
        delay=delayMilli;
    }
    std::thread start() {
        return std::thread([=] { run(); });
    }
    void buildCompound(){
        drop0.linkSuccessor(&queue0);
        queue0.linkSuccessor(&pass1);
        pass1.linkSuccessor(&queue1);
        queue1.linkSuccessor(&pass2);
        pass2.linkSuccessor(&drop1);

    }
    void breakCompound(){
        SCUD_RC status=SCUD_RC_OK;
        Linkable<int,void*>::LinkedObjectsTuple lot1=drop1.unlink(&status);
        if(status!=SCUD_RC_OK){
            std::cout<<SCHelper::convertReturnCodeToString(status)<<std::endl;
        }
        Linkable<int,void*>::LinkedObjectsTuple lot2=queue0.unlink(&status);
        if(status!=SCUD_RC_OK){
            std::cout<<SCHelper::convertReturnCodeToString(status)<<std::endl;
        }
        Linkable<int,void*>::LinkedObjectsTuple lot3=queue1.unlink(&status);
        if(status!=SCUD_RC_OK){
            std::cout<<SCHelper::convertReturnCodeToString(status)<<std::endl;
        }
    }
    Linkable<int,void*>* compoundBegin(){
        return &drop0;
    }
    Linkable<int,void*>* compoundEnd(){
        return &drop1;
    }
};

class Sink{
    //LinkableSchedulerPriority<int, void*> scheduler;
    //LinkableSchedulerNaiveRR<int, void*> scheduler;
    LinkableSchedulerDRR<int, void*> scheduler;
    LinkableNull<int, void*> null;
    void run(){
        scheduler.setPriority(9);
        scheduler.setWeight(1);
        
//        while(1){
//            //std::cout<<"loop: sink"<<std::endl;
//            std::this_thread::sleep_for(std::chrono::milliseconds(100));
//            SCUD_RC rc=SCUD_RC_OK;
//            struct Linkable<int,void*>::Queueable res;
////            rc=next->pull(res);
////            if(rc==SCUD_RC_OK){
////                std::cout<<"Sink: Pulled object "<<res.scheduled<<" "<<std::endl;
////            }else{
////                std::cout<<SCHelper::convertReturnCodeToString(rc)<<std::endl;
////            }
////            if(res.scheduled>100000){
////                int x=0;
////            }
//        }
    }
public:
    Sink(Linkable<int, void*>* l){
        
    }
    std::thread start() {
        return std::thread([=] { run(); });
    }
    void buildCompound(){
        scheduler.linkSuccessor(&null);
    };
    void breakCompound(){
        SCUD_RC status=SCUD_RC_OK;
        null.unlink(&status);
    };
    Linkable<int,void*>* compoundBegin(){
        return &scheduler;
    }
    Linkable<int,void*>* compoundEnd(){
        return &null;
    }
    
};
class Mixer{
    std::vector<Source*> v;
    Sink* sink;
    void run()
    {
        SCUD_RC rc=SCUD_RC_OK;
        sink->breakCompound();
        //std::cout<<"loop: mixer"<<std::endl;
        int s=v.size();
        //std::cout<<"loop: mixer 1"<<std::endl;
        for(int i=0;i<s;i++){
            v[i]->breakCompound();
            pull();
        }
        sink->buildCompound();
        for(int i=0;i<s;i++){
            v[i]->buildCompound();
            sink->compoundBegin()->linkPredecessor(v[i]->compoundEnd());
        }
        
        
        while (1) {
            //std::cout<<"loop: mixer 0"<<std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            std::cout<<"** ROUND **"<<std::endl;
            for(int j=0;j<100;++j)
            {
                
                pull();
                
                
            }
        }
    }

public:
    void addSource(Source* s){
        v.push_back(s);
    }
    void addSink(Sink* s){
        sink=s;
    }
    void addNull(){
    
    };
    void pull(){
        SCUD_RC rc=SCUD_RC_OK;
        struct Linkable<int,void*>::Queueable res;
        res.scheduled=-1;
        auto start= std::chrono::high_resolution_clock::now() ;
        rc=sink->compoundEnd()->pull(res);
        auto end= std::chrono::high_resolution_clock::now() ;
        auto diff = end - start;
        //std::cout<<"Pull delay="<<std::chrono::duration <double, std::micro>(diff).count()<<std::endl;
        
        if(rc==SCUD_RC_OK){
            std::cout<<"---Mixer: Pulled object "<<res.scheduled<<" "<<std::endl;
        }else if(rc==SCUD_RC_FAIL_LINK_NO_PACKET_AVAILABLE){
            std::cout<<SCHelper::convertReturnCodeToString(rc)<<std::endl;
        }
    }
    std::thread start() {
        return std::thread([=] { run(); });
    }
};
int main()
{
    Source s1(1,10,1,100);
    //Source s2(2,20,1,100);
    //Source s3(3,30,1,100);
    //Source s4(5,50,1,1000);
    
    Mixer m;
    m.addSource(&s1);
    //m.addSource(&s2);
    //m.addSource(&s3);
    //m.addSource(&s4);
    
    std::cout<<"SW version is: "<<SCHelper::version()<<std::endl;
    LinkableNull<int, void*> null;
    Sink si0(0);
    
    
    m.addSink(&si0);
    
    std::thread t1=s1.start();
    //std::thread t2=s2.start();
    //std::thread t3=s3.start();
    //std::thread t4=s4.start();
    std::thread ti0=si0.start();
    std::thread m0=m.start();
    
    t1.join();
    //t2.join();
    //t3.join();
    //t4.join();
    ti0.join();
    m0.join();
    std::cout << "done!\n";
}

