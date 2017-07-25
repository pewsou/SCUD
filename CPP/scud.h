//
//  Scud.h
//
//  Created by Boris Vigman on 17/05/2017.
//  Copyright © 2017 Boris Vigman. All rights reserved.
//
/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef Scud_h
#define Scud_h

#include "deque"
#include "map"
#include <iostream>

namespace SCUD{

#define SCUD_VERSION "0.1.3"
//#define SCUD_USE_EXCEPTIONS 1
#define SCUD_MAX_NUMBER_OF_AVAILABLE_PRIORITIES 64
//#define SCUD_DEBUG_MODE_ENABLED
#define SCUD_CUSTOM_MUTEX_AVAILABLE
#define SCUD_CUSTOM_RNG_AVAILABLE
//#define SCUD_CUSTOM_QUEUE_AVAILABLE
    
//-------------------------------------------------------
//         PLEASE DO NOT EDIT BELOW THIS LINE
//-------------------------------------------------------
#define SCUD_DROPPER_RANDOM_NUMBERS_AMOUNT 8
#ifdef SCUD_USE_EXCEPTIONS
#define SCUD_THROW_EXCEPTION(x) throw (x)
#else
  #define SCUD_THROW_EXCEPTION(x)
#endif
    
#ifdef SCUD_DEBUG_MODE_ENABLED
#include <string>

#define SCUD_PRINT_STR(x) {std::string s=(x);std::cout<<s<<std::endl;}
#define SCUD_PRINT_STR_WITH_PARAM(x,param) {std::string s=(x);std::cout<<s<<(param)<<std::endl;}
#else
#define SCUD_PRINT_STR(x)
#define SCUD_PRINT_STR_WITH_PARAM(x,param)
#endif

#define SCUD_MAX_POSSIBLE_PRIORITY 127
#if SCUD_MAX_POSSIBLE_PRIORITY+1 < SCUD_MAX_NUMBER_OF_AVAILABLE_PRIORITIES
#error "Maximum available priority is greater than maximum allowed priority"
#endif
typedef enum {
    SCUD_RC_OK=0,
    SCUD_RC_FAIL_INVALID_PARAM,
    SCUD_RC_FAIL_INVALID_PRIORITY,
    SCUD_RC_FAIL_INVALID_WEIGHT,
    SCUD_RC_FAIL_LINK_EXISTS,
    SCUD_RC_FAIL_LINK_NOT_EXISTS,
    SCUD_RC_FAIL_SELF_REFERENCING_LINK_DETECTED,
    SCUD_RC_FAIL_ILLEGAL_OP,
    SCUD_RC_LINK_FAILED,
    SCUD_RC_FAIL_LINK_HAS_PREDECESSOR,
    SCUD_RC_FAIL_LINK_HAS_SUCCESSOR,
    SCUD_RC_FAIL_LINK_ID_NOT_SET,
    SCUD_RC_FAIL_LINK_UNDER_LOW_THRESHOLD,
    SCUD_RC_FAIL_LINK_ABOVE_HIGH_THRESHOLD,
    SCUD_RC_FAIL_LINK_NO_PACKET_AVAILABLE,
    SCUD_RC_FAIL_OBJ_PROPAGATION_FAILED
} SCUD_RC;


typedef struct _Prim{
    SCUD_RC retCode;
    
    union {
        long long signedInt;
        long Long;
        short Short;
        long double real;
        unsigned long long unsignedInt;
        unsigned long unsignedLong;
        unsigned short unsignedShort;
        
        struct{
            char* ptr;
            long long size;
        } ptr;
        
    } primitives;
    _Prim(){
        retCode=SCUD_RC_OK;
    }
} SchedulablePayload;

//#ifndef SCUD_CUSTOM_QUEUE_AVAILABLE
//#include "mutex"
//    template<typename TSchedulable>class CQueue{
//    public:
//        CQueue(){};
//        void push(){};
//        
//        long long size();
//        virtual ~CQueue(){};
//    };
//#else
//    template<typename TSchedulable>class CQueue{
//    public:
//        CQueue(){};
//        
//        long long size();
//        virtual ~CQueue(){};
//    };
//#endif
#ifndef SCUD_CUSTOM_RNG_AVAILABLE
#include <stdlib.h>
#include <time.h>
    class SCRng{
    public:
        SCRng(){
            srand ((unsigned int)time(NULL));
        };
        //returns random number in range 0..RAND_MAX
        int randomInt(){
            
            return rand();
        };
        //returns random number in range (0..1)
        float randomFloat(){
            return rand()/((float)RAND_MAX);
        };
        virtual ~SCRng(){};
    };
#else
    class SCRng{
    public:
        SCRng();
        //returns random number in range 0..RAND_MAX
        int randomInt();
        //returns random number in range (0..1)
        float randomFloat();
        virtual ~SCRng();
    };
#endif
#ifndef SCUD_CUSTOM_MUTEX_AVAILABLE
#include "mutex"
class SCLocker{
    std::mutex mut;
public:
    SCLocker(){};
    void lock(){
        mut.lock();
    };
    void unlock(){
        mut.unlock();
    };
    virtual ~SCLocker(){};
};
#else
class SCLocker{
public:
    SCLocker();
    void lock();
    void unlock();
    virtual ~SCLocker();
};
#endif
class SCHelper{
    static char itsVersion[];
public:
    static char* version(){
        return itsVersion;
    }
    static std::string convertReturnCodeToString(SCUD_RC val){
        std::string result="Result: undefined";
        switch (val) {
            case SCUD_RC_OK:result="Result: OK";break;
            case SCUD_RC_FAIL_INVALID_PARAM:result="Result: FAIL: invalid param provided";break;
            case SCUD_RC_FAIL_INVALID_PRIORITY:result="FAIL: invalid priority";break;
            case SCUD_RC_FAIL_INVALID_WEIGHT:result="FAIL: invalid weight";break;
            case SCUD_RC_FAIL_LINK_EXISTS:result="FAIL: link already exists";break;
            case SCUD_RC_FAIL_LINK_NOT_EXISTS:result="FAIL: link not exists";break;
            case SCUD_RC_FAIL_SELF_REFERENCING_LINK_DETECTED:result="Result: Self referencing link";break;
            case SCUD_RC_FAIL_ILLEGAL_OP:result="FAIL: illegal operation requested";break;
            case SCUD_RC_LINK_FAILED:result="FAIL: link operation failed";break;
            case SCUD_RC_FAIL_LINK_HAS_PREDECESSOR:result="FAIL: predecessor exists";break;
            case SCUD_RC_FAIL_LINK_HAS_SUCCESSOR:result="FAIL: successor exists";break;
            case SCUD_RC_FAIL_LINK_ID_NOT_SET:result="FAIL: link ID not set";break;
            case SCUD_RC_FAIL_LINK_UNDER_LOW_THRESHOLD:result="FAIL: under low threshold";break;
            case SCUD_RC_FAIL_LINK_ABOVE_HIGH_THRESHOLD:result="FAIL: above high threshold";break;
            case SCUD_RC_FAIL_LINK_NO_PACKET_AVAILABLE:result="FAIL: no object is available";break;
            case SCUD_RC_FAIL_OBJ_PROPAGATION_FAILED:result="FAIL: object propagation failed";break;
            default:
                break;
        }
        return result;
    }
};
template<typename TSchedulable,typename Tid> class LinkableDropper ;
template<typename TSchedulable,typename Tid> class LinkableQueue;
template<typename TSchedulable,typename Tid> class LinkableScheduler ;
template<typename TSchedulable,typename Tid> class LinkableSchedulerPriority;
template<typename TSchedulable,typename Tid> class LinkableSchedulerNaiveRR;
template<typename TSchedulable,typename Tid> class LinkableNull;
template<typename TSchedulable,typename Tid> class LinkablePass;
    /*
     -----------------------------------------------------------------------------
     LINKABLE
     
     -----------------------------------------------------------------------------
     
     */
template<typename TSchedulable, typename Tid> class Linkable{
protected:
#ifdef SCUD_DEBUG_MODE_ENABLED
    std::string elementClass;
#endif
    long highT;
    long lowT;
    Tid itsId;
    SCLocker lockerLinkable;
public:

    struct Queueable{
        TSchedulable scheduled;
        long long schParam;
        Queueable(){schParam=-1;};
    };
protected:
    struct SchedulingProperties{
        float weight;
        char priority;
        SchedulingProperties(){weight=-1;priority=-1;}
        SchedulingProperties(float w,char p){weight=w;priority=p;}
    };

    friend class LinkableQueue<TSchedulable,Tid>;
    friend class LinkableDropper<TSchedulable,Tid>;
    friend class LinkableScheduler<TSchedulable,Tid>;
    friend class LinkableSchedulerPriority<TSchedulable,Tid>;
    friend class LinkableNull<TSchedulable,Tid>;
    friend class LinkablePass<TSchedulable,Tid>;
    
    Linkable<TSchedulable,Tid>* next;
    Linkable<TSchedulable,Tid>* prev;
    SchedulingProperties scp;
    Tid getId(){
        //lockerId.lock();
        Tid t=itsId;
        //lockerId.unlock();
        return t;
    };
    void setId(Tid tid){
        //lockerId.lock();
        itsId=tid;
        //lockerId.unlock();
    };
    virtual void _signalAvailability(bool canPull, long long countAvailable, float weight,char priority)=0;
    virtual SCUD_RC _pull(struct Linkable<TSchedulable,Tid>::Queueable& qu)=0;
    virtual SCUD_RC _unlinkPredecessor(Linkable<TSchedulable,Tid>*  link){
        SCUD_PRINT_STR("enter Linkable::_unlinkPredecessor");
        lockerLinkable.lock();
        prev=0;
        lockerLinkable.unlock();
        SCUD_PRINT_STR("exit Linkable::_unlinkPredecessor");
        return SCUD_RC_OK;
    };
    virtual SCUD_RC _unlinkSuccessor(Linkable<TSchedulable,Tid>*  link){
        SCUD_PRINT_STR("enter Linkable::_unlinkSuccessor");
        lockerLinkable.lock();
        next=0;
        lockerLinkable.unlock();
        SCUD_PRINT_STR("exit Linkable::_unlinkSuccessor");
        return SCUD_RC_OK;
    };
    virtual SCUD_RC _linkPredecessor(Linkable<TSchedulable,Tid>*  link){
        SCUD_PRINT_STR("enter Linkable::_linkPredecessor");
        lockerLinkable.lock();
        prev=link;
        lockerLinkable.unlock();
        SCUD_PRINT_STR("exit Linkable::_linkPredecessor");
        return SCUD_RC_OK;
    };
    virtual SCUD_RC _linkSuccessor(Linkable<TSchedulable,Tid>*  link){
        SCUD_PRINT_STR("enter Linkable::_linkSuccessor");
        lockerLinkable.lock();
        next=link;
        lockerLinkable.unlock();
        SCUD_PRINT_STR("exit Linkable::_linkSuccessor");
        return SCUD_RC_OK;
    };
    virtual SCUD_RC _propagateSchedulingProperties(Linkable<TSchedulable,Tid>*  link,SchedulingProperties scps){
        return SCUD_RC_OK;
    };
    virtual SCUD_RC _prePull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
        struct Linkable<TSchedulable,Tid>::Queueable temp;
        temp.schParam=-1;
        SCUD_RC rc=SCUD_RC_OK;
        qu=temp;
        return rc;
    };

public:
    struct LinkedObjectsTuple {
        std::vector<Linkable<TSchedulable,Tid>* > prevObject;
        Linkable<TSchedulable,Tid>* thisObject;
        Linkable<TSchedulable,Tid>* nextObject;
        LinkedObjectsTuple(Linkable<TSchedulable,Tid>* p,Linkable<TSchedulable,Tid>* t,Linkable<TSchedulable,Tid>* n){
            if(p){
                prevObject.push_back(p);
            }
            thisObject=t;
            nextObject=n;
        };
    } ;
    
    Linkable():lowT(0),highT(0),next(0),prev(0){
        //objects.reserve(1024);
        scp.weight=0;
        scp.priority=-1;
#ifdef SCUD_DEBUG_MODE_ENABLED
        elementClass="Linkable";
#endif
    };
    //TODO: Consider Removal
    Linkable(unsigned long initialObjCount):lowT(0),highT(0),next(0),prev(0){
#ifdef SCUD_DEBUG_MODE_ENABLED
        elementClass="Linkable";
#endif
        scp.weight=0;
        scp.priority=-1;
        //objects.reserve(initialObjCount);
    };
    virtual ~Linkable(){
        SCUD_PRINT_STR("enter Linkable::~Linkable");
        lockerLinkable.lock();
        prev=0;
        next=0;
        lockerLinkable.unlock();
        SCUD_PRINT_STR("exit Linkable::~Linkable");
    };
    //pull object from previous link if such exists
    virtual SCUD_RC pull(struct Linkable<TSchedulable,Tid>::Queueable& qu)=0;
    virtual void process(TSchedulable sch, long long schedulingParam){
        
    };
    //Pull object if available from previous link if such exists and push to next link if exists
    virtual SCUD_RC pullAndPush(){
        SCUD_RC retcode=SCUD_RC_OK;
        SCUD_PRINT_STR("enter Linkable::pullAndPush");
        struct Linkable<TSchedulable,Tid>::Queueable ts;
        retcode=this->pull(ts);
        lockerLinkable.lock();
        Linkable<TSchedulable,Tid>* n=this->next;
        lockerLinkable.unlock();
        if(retcode==SCUD_RC_OK && n){
            this->process(ts.scheduled,ts.schParam);
            n->push(ts.scheduled,ts.schParam);
            
        }else{
            SCUD_PRINT_STR("FAIL Linkable::pullAndPush");
            retcode=SCUD_RC_FAIL_OBJ_PROPAGATION_FAILED;
        }
        
        SCUD_PRINT_STR("exit Linkable::pullAndPush");
        return retcode;
    };
    //Push object along with scheduling property
    virtual SCUD_RC push(TSchedulable sch, long long schedulingParam)=0;
    virtual bool canPull()=0;
    virtual bool hasBefore(){
        SCUD_PRINT_STR("enter Linkable::hasBefore");
        bool res=false;
        lockerLinkable.lock();
        if(this->prev){
            res=true;
        }
        lockerLinkable.unlock();
        SCUD_PRINT_STR("exit Linkable::hasBefore");
        return res;
    }
    virtual bool hasAfter(){
        SCUD_PRINT_STR("enter Linkable::hasAfter");
        bool res=false;
        lockerLinkable.lock();
        if(this->next){
            res=true;
        }
        lockerLinkable.unlock();
        SCUD_PRINT_STR("exit Linkable::hasAfter");
        return res;
    }
    SCUD_RC setHighThreshold(long high){
        if(high<0)
            return SCUD_RC_FAIL_INVALID_PARAM;
        lockerLinkable.lock();
        if(lowT>high && high>0){
            lockerLinkable.unlock();
            return SCUD_RC_FAIL_INVALID_PARAM;
        }
        highT=high;
        lockerLinkable.unlock();
        return SCUD_RC_OK;
    };
    SCUD_RC setLowThreshold(long low){
        if(low<0)
            return SCUD_RC_FAIL_INVALID_PARAM;
        lockerLinkable.lock();
        if(low>highT && highT>0){
            lockerLinkable.unlock();
            return SCUD_RC_FAIL_INVALID_PARAM;
        }
        lowT=low;
        lockerLinkable.unlock();
        return SCUD_RC_OK;
    };
    virtual SCUD_RC setWeight(float w){
        if(w<0){
            SCUD_PRINT_STR("Linkable::setWeight - Attepmt of setting invalid weight:new weight is less than 0");
            return SCUD_RC_FAIL_INVALID_WEIGHT;
        }
         //setWeight(weight);
        lockerLinkable.lock();
        if(next){
            typename Linkable<TSchedulable, Tid>::SchedulingProperties scps;
            scps.weight=w;
            scps.priority=this->scp.priority;
            next->_propagateSchedulingProperties(this,this->scp);
        }
        lockerLinkable.unlock();
        return SCUD_RC_OK;
    };
    virtual SCUD_RC setPriority( char prio){
        if(prio<0 || prio>=SCUD_MAX_NUMBER_OF_AVAILABLE_PRIORITIES){
            SCUD_PRINT_STR("Linkable::setPriority - Attepmt of setting invalid priority:");
            SCUD_PRINT_STR("new priority is bigger than maximum possible or is less than 0");
            SCUD_PRINT_STR_WITH_PARAM("maximum possible priority is ", SCUD_MAX_NUMBER_OF_AVAILABLE_PRIORITIES-1);
            return SCUD_RC_FAIL_INVALID_PRIORITY;
        }
        
        lockerLinkable.lock();
        this->scp.priority=prio;
        if(next){
            typename Linkable<TSchedulable, Tid>::SchedulingProperties scps;
            scps.weight=this->scp.weight;
            scps.priority=this->scp.priority;
            next->_propagateSchedulingProperties(this,this->scp);
        }
        lockerLinkable.unlock();
        return SCUD_RC_OK;
    };
    double getWeight(){
        lockerLinkable.lock();
        double weight=this->scp.weight;
        lockerLinkable.unlock();
        return weight;
    };
    char getPriority(){
        lockerLinkable.lock();
         char priority=this->scp.priority;
        lockerLinkable.unlock();
        return priority;
    };

    //Remove given element from chain. Rules of removal for
    //Scheduler: removes given scheduler itself and all prepended elements
    //Dropper: removes only given Dropper itself
    //Queue: removes only given Queue
    virtual LinkedObjectsTuple unlink(SCUD_RC* rc) {
        SCUD_PRINT_STR("enter Linkable::unlink");
        LinkedObjectsTuple ptn(0,this,0) ;
        lockerLinkable.lock();
        if(next!=0){
            SCUD_PRINT_STR("Linkable::unlink - next not empty");
            ptn.nextObject=next;
            next->_unlinkPredecessor(this);
            next=0;
        }
        if(prev!=0){
            SCUD_PRINT_STR("Linkable::unlink - prev not emty");
            ptn.prevObject.push_back(prev);
            prev->_unlinkSuccessor(this);
            prev=0;
        }
        lockerLinkable.unlock();
        SCUD_PRINT_STR("exit Linkable::unlink");
        
        *rc=SCUD_RC_OK;
        return ptn;
    };
    //insert this object after the specified element
    virtual SCUD_RC linkSuccessor(Linkable<TSchedulable,Tid>* link) {
        SCUD_PRINT_STR("enter Linkable::linkSuccessor");
        if(link==this){
            SCUD_PRINT_STR("exit Linkable::linkSuccessor - self referencing link");
            SCUD_THROW_EXCEPTION("exit Linkable::linkSuccessor - self referencing link");
            return SCUD_RC_FAIL_SELF_REFERENCING_LINK_DETECTED;
        }
        if(link==0){
            SCUD_PRINT_STR("exit Linkable::linkSuccessor - invalid param");
            SCUD_THROW_EXCEPTION("exit Linkable::linkSuccessor - invalid param");
            return SCUD_RC_FAIL_INVALID_PARAM;
        }
        
        if(link->hasBefore()){
            SCUD_PRINT_STR("exit Linkable::linkSuccessor - has predecessor");
            SCUD_THROW_EXCEPTION("exit Linkable::linkSuccessor - has predecessor");
            return SCUD_RC_FAIL_LINK_HAS_PREDECESSOR;
        }
        if(hasAfter()){
            SCUD_PRINT_STR(" Linkable::linkSuccessor - next is NOT empty");
        }else
        //if(next==0)
        {
            SCUD_RC rc=SCUD_RC_OK;
            SCUD_PRINT_STR(" Linkable::linkSuccessor - next is empty");
            
            if(link->_linkPredecessor(this)==SCUD_RC_OK){
                lockerLinkable.lock();
                next=link;
                lockerLinkable.unlock();
                SCUD_PRINT_STR("exit Linkable::linkSuccessor - OK");
            }else{
                rc=SCUD_RC_LINK_FAILED;
                SCUD_PRINT_STR("exit Linkable::linkSuccessor - link failed");
                SCUD_THROW_EXCEPTION("exit Linkable::linkSuccessor - link failed");
            }
            
            return rc;
        }
        SCUD_PRINT_STR("exit Linkable::linkSuccessor - link exists");
        SCUD_THROW_EXCEPTION("exit Linkable::linkSuccessor - link exists");
        return SCUD_RC_FAIL_LINK_EXISTS;
    };
    virtual SCUD_RC linkPredecessor(Linkable<TSchedulable,Tid>* link){
        SCUD_PRINT_STR("enter Linkable::linkPredecessor - link exists");
        if(link==this){
            SCUD_PRINT_STR("exit Linkable::linkPredecessor - self referencing link");
            SCUD_THROW_EXCEPTION("exit Linkable::linkPredecessor - self referencing link");
            return SCUD_RC_FAIL_SELF_REFERENCING_LINK_DETECTED;
        }
        if(link==0){
            SCUD_PRINT_STR("exit Linkable::linkPredecessor - invalid link");
            SCUD_THROW_EXCEPTION("exit Linkable::linkPredecessor - invalid link");
            return SCUD_RC_FAIL_INVALID_PARAM;
        }
        if(link->hasAfter()){
            SCUD_PRINT_STR("exit Linkable::linkPredecessor - has sucessor");
            SCUD_THROW_EXCEPTION("exit Linkable::linkPredecessor - has sucessor");
            return SCUD_RC_FAIL_LINK_HAS_SUCCESSOR;
        }
        if(link->hasBefore()){
            
        }else
        //if(prev==0)
        {
            SCUD_PRINT_STR(" Linkable::linkSuccessor - prev is empty");
            lockerLinkable.lock();
            prev=link;
            prev->_linkSuccessor(this);
            lockerLinkable.unlock();
            SCUD_PRINT_STR("exit Linkable::linkPredecessor - OK");
            return SCUD_RC_OK;
        }
        SCUD_PRINT_STR("exit Linkable::linkPredecessor - link exists");
        SCUD_THROW_EXCEPTION("exit Linkable::linkPredecessor - link exists");
        return SCUD_RC_FAIL_LINK_EXISTS;
    };

};
    /*
     -----------------------------------------------------------------------------
      NULL OBJECT
     
     -----------------------------------------------------------------------------
     
     */
template<typename TSchedulable,typename Tid> class LinkableNull :public Linkable<TSchedulable,Tid>{
    //Locker lockerNull;
protected:
    SCUD_RC _pull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
        SCUD_RC retcode=SCUD_RC_OK;
        SCUD_PRINT_STR("enter LinkableNull::_pull");
        this->lockerLinkable.lock();
        Linkable<TSchedulable,Tid>* p=this->prev;
        this->lockerLinkable.unlock();
        if(p && _canPull())
        {
            SCUD_PRINT_STR("+LinkableNull::_pull");
            retcode=p->_pull(qu);
            SCUD_PRINT_STR("-LinkableNull::_pull");
        }else{
            retcode=SCUD_RC_FAIL_LINK_NO_PACKET_AVAILABLE;
        }
        
        SCUD_PRINT_STR("exit LinkableNull::_pull");
        return retcode;
    }
    void _signalAvailability(bool canPull,long long countAvailable, float weight,char priority){
        SCUD_PRINT_STR("LinkableNull::_signalAvailability");
    }
    bool _canPull(){
        bool res=false;
        
        SCUD_PRINT_STR("enter LinkableNull::_canPull");
        this->lockerLinkable.lock();
        res= this->prev->canPull();
        this->lockerLinkable.unlock();
        SCUD_PRINT_STR("exit LinkableNull::_canPull");
        return res;
    }
public:
    LinkableNull(){
#ifdef SCUD_DEBUG_MODE_ENABLED
        this->elementClass="Null";
#endif
    }
    SCUD_RC push(TSchedulable sch, long long schedulingParam){
        SCUD_PRINT_STR("enter LinkableNull::push");
        SCUD_PRINT_STR("enter LinkableNull::push");
        return SCUD_RC_OK;
    }
    SCUD_RC pull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
        SCUD_RC retcode=SCUD_RC_OK;
        SCUD_PRINT_STR("enter LinkableNull::pull");
        retcode= this->_pull(qu);
        SCUD_PRINT_STR("exit LinkableNull::pull");
        return retcode;
    }
    bool canPull(){
        SCUD_PRINT_STR("enter LinkableNull::canPull");
        this->lockerLinkable.lock();
        bool res=this->_canPull();
        this->lockerLinkable.unlock();
        SCUD_PRINT_STR("exit LinkableNull::canPull");
        return res;
    }
};
   /*
        ---------------------------------------------------------------------------
                                DROPPER
        ---------------------------------------------------------------------------
     */
template<typename TSchedulable,typename Tid> class LinkableDropper :public Linkable<TSchedulable,Tid>{
    float randoms[SCUD_DROPPER_RANDOM_NUMBERS_AMOUNT];
    int currentRandom;
    float droppingProbability;
    SCRng rng;
protected:
    bool _shouldDrop(TSchedulable sch, long long schedulingParam){
        bool res=true;
        this->lockerLinkable.lock();
        currentRandom=(currentRandom+1)%SCUD_DROPPER_RANDOM_NUMBERS_AMOUNT ;
        if(randoms[currentRandom]>droppingProbability){
            res=false;
        }
        this->lockerLinkable.unlock();
        randoms[currentRandom]=rng.randomFloat();
        return res;
    }
    SCUD_RC _pull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
        SCUD_RC retcode=SCUD_RC_OK;
        this->lockerLinkable.lock();
        Linkable<TSchedulable,Tid>* p=this->prev;
        this->lockerLinkable.unlock();
        if(p && canPull())
        {
            retcode=p->_pull(qu);
            
        }else {
            retcode=SCUD_RC_FAIL_LINK_NO_PACKET_AVAILABLE;
        }

        return retcode;
    }
    void _signalAvailability(bool canPull,long long countAvailable,float weight,char priority){
        
    }
    void initializeRandomSamples(){
        currentRandom=0;
        droppingProbability=-1;
        for (int i=0;i<SCUD_DROPPER_RANDOM_NUMBERS_AMOUNT;++i) {
            randoms[i]=rng.randomFloat();
        }
        
    }
public:
    LinkableDropper(Tid tid){
#ifdef SCUD_DEBUG_MODE_ENABLED
        this->elementClass="Dropper";
#endif
        this->setId(tid);
        initializeRandomSamples();
    };
    LinkableDropper(){
#ifdef SCUD_DEBUG_MODE_ENABLED
        this->elementClass="Dropper";
#endif
        this->setId(this);
        initializeRandomSamples();
    };

    //Probablity of dropping object; must be in range [0..1]
    SCUD_RC setDroppingProbability(float probability){
        if(probability<0||probability>1){
            return SCUD_RC_FAIL_INVALID_PARAM;
        }else{
            this->lockerLinkable.lock();
            this->droppingProbability=probability;
            this->lockerLinkable.unlock();
            return SCUD_RC_OK;
        }
    }
    SCUD_RC pull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
        SCUD_RC retcode=SCUD_RC_OK;
        struct Linkable<TSchedulable,Tid>::Queueable pp;
        this->lockerLinkable.lock();
        Linkable<TSchedulable,Tid>* p=this->prev;
        this->lockerLinkable.unlock();
        if(p){
            retcode=p->_prePull(pp);
            if(retcode==SCUD_RC_OK && this->shouldDrop(pp.scheduled,pp.schParam)==false){
                retcode=p->_pull(qu);
            }
        }else{
            retcode=SCUD_RC_FAIL_LINK_NOT_EXISTS;
        }
        
        return retcode;
    }
    SCUD_RC push(TSchedulable sch, long long schedulingParam){
        SCUD_RC retcode=SCUD_RC_OK;
        this->lockerLinkable.lock();
        Linkable<TSchedulable,Tid>* n=this->next;
        this->lockerLinkable.unlock();
        if(n){
            if(this->shouldDrop(sch,schedulingParam)==false){
                retcode=n->push(sch,schedulingParam);
            }
        }else{
            retcode=SCUD_RC_FAIL_LINK_NOT_EXISTS;
        }
        
        return retcode;
    }
    SCUD_RC setWeight(float w){
        this->lockerLinkable.lock();
        Linkable<TSchedulable, Tid>* n=this->next;
        this->lockerLinkable.unlock();
        
        if(n!=0){
            n->setWeight(w);
        }
        
        return SCUD_RC_OK;
    };
    virtual bool canPull(){
        this->lockerLinkable.lock();
        Linkable<TSchedulable,Tid>* p=this->prev;
        this->lockerLinkable.unlock();
        bool rc= p->canPull();
        
        return rc;
    }
    virtual bool shouldDrop(TSchedulable sch, long long schedulingParam){
        return _shouldDrop(sch, schedulingParam);
    }
};
/*
 -------------------------------------------------------------------------
                QUEUE
 -------------------------------------------------------------------------
 */
template<typename TSchedulable,typename Tid> class LinkableQueue :public Linkable<TSchedulable,Tid>{
    std::deque<struct Linkable<TSchedulable,Tid>::Queueable> queue;
protected:
    SCUD_RC _prePull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
        struct Linkable<TSchedulable,Tid>::Queueable temp;
        temp.schParam=-1;
        SCUD_RC rc=SCUD_RC_OK;
        this->lockerLinkable.lock();
        if(queue.size()>this->lowT){
            qu=this->queue.back();
            rc=SCUD_RC_OK;
        }else{
            qu=temp;
            rc=SCUD_RC_FAIL_LINK_UNDER_LOW_THRESHOLD;
        }
        this->lockerLinkable.unlock();
        return rc;
    }
    SCUD_RC _pull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
        //struct Linkable<TSchedulable,Tid>::Queueable ts;
        SCUD_RC retcode=SCUD_RC_OK;
        this->lockerLinkable.lock();
        long long qs=queue.size();
        Linkable<TSchedulable,Tid>* n=this->next;
        if(qs>0){
            if(this->lowT>=0 && qs>this->lowT)
            {
                qu=this->queue.back();
                this->queue.pop_back();
                this->lockerLinkable.unlock();
                if(qs-1<=this->lowT){
                    if(n){
                        n->_signalAvailability(false,qs-1,this->scp.weight,this->scp.priority);
                    }
                }
            }else{
                this->lockerLinkable.unlock();
                if(n){
                    n->_signalAvailability(false,qs,this->scp.weight,this->scp.priority);
                }

               retcode=SCUD_RC_FAIL_LINK_UNDER_LOW_THRESHOLD;
            }
        }else{
            this->lockerLinkable.unlock();
            if(n){
                n->_signalAvailability(false,0,this->scp.weight,this->scp.priority);
            }
            retcode=SCUD_RC_FAIL_LINK_NO_PACKET_AVAILABLE;
        }
        
        return retcode;
    }
    void _signalAvailability(bool canPull,long long countAvailable,float weight,char priority){
        
        this->lockerLinkable.lock();
        Linkable<TSchedulable,Tid>* n=this->next;
        this->lockerLinkable.unlock();
        if(n){
            n->_signalAvailability(canPull, countAvailable,this->scp.weight,this->scp.priority);
        }
        
    }
public:
    LinkableQueue(Tid tid){
#ifdef SCUD_DEBUG_MODE_ENABLED
        this->elementClass="Queue";
#endif
        this->setId(tid);
    };
    LinkableQueue(){
#ifdef SCUD_DEBUG_MODE_ENABLED
        this->elementClass="Queue";
#endif
        this->setId(this);
    };
    
    SCUD_RC pull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
        SCUD_RC retcode=SCUD_RC_OK;
        retcode= this->_pull(qu);
        return retcode;
    }
    SCUD_RC push(TSchedulable sch, long long schedulingParam){
        SCUD_RC res=SCUD_RC_OK;
        this->lockerLinkable.lock();
        Linkable<TSchedulable,Tid>* n=this->next;
        long long qs=queue.size();
        long hT=this->highT,lT=this->lowT;
        if(hT>0 && qs<hT){
            struct Linkable<TSchedulable,Tid>::Queueable q;
            q.scheduled=sch;
            q.schParam=schedulingParam;
            this->queue.push_front(q);
            this->lockerLinkable.unlock();
            if(n && qs+1>lT && qs-lT<2){
                n->_signalAvailability(true,qs+1,this->scp.weight,this->scp.priority);
            }
//            else{
//                this->next->_signalAvailability(false,qs+1);
//            }
        }else if(qs>=hT){
            this->lockerLinkable.unlock();
            res=SCUD_RC_FAIL_LINK_ABOVE_HIGH_THRESHOLD;
        }
        
        
        return res;
    }
    virtual SCUD_RC empty(){
        this->lockerLinkable.lock();
        queue.empty();
        this->lockerLinkable.unlock();
        return SCUD_RC_OK;
    };
    bool canPull(){
        bool res=false;
        this->lockerLinkable.lock();
        long long qs=queue.size();
        if(qs>this->lowT){
            res=true;
        }
        this->lockerLinkable.unlock();
        return res;
    }
    long long size(){
        this->lockerLinkable.lock();
        long long _size=queue.size();
        this->lockerLinkable.unlock();
        return _size;
    }
};
/*
 ---------------------------------------------------------------------------------
                GENERIC SCHEDULER
 ---------------------------------------------------------------------------------
 
 */
template<typename TSchedulable,typename Tid> class LinkableScheduler :public Linkable<TSchedulable,Tid>{
protected:
    struct InternalContainer{
        typename Linkable<TSchedulable,Tid>::SchedulingProperties scps;
        Linkable<TSchedulable,Tid>* link;
        InternalContainer(){
            scps.weight=-1;
            scps.priority=-1;
            link=0;
        }
        InternalContainer(Linkable<TSchedulable,Tid>* l, double weight, char priority){
            scps.weight=weight;
            scps.priority=priority;
            link=l;
        }
    };
    std::map<Tid,InternalContainer> id2prepended ;
    virtual Linkable<TSchedulable,Tid>* calculateNextSource(bool pktsEnded)=0;
    virtual void _releaseScheduledEntry(Tid linkId,Linkable<TSchedulable,Tid>* link,float weight,char priority)=0;
    virtual bool _scheduleEntry(Tid linkId,Linkable<TSchedulable,Tid>* link,float weight,char priority)=0;
    virtual bool _scheduleFinalizeEntry(Tid linkId,Linkable<TSchedulable,Tid>* link)=0;
    SCUD_RC _unlinkPredecessor(Linkable<TSchedulable,Tid>*  link){
        SCUD_PRINT_STR("enter LinkableScheduler::_unlinkPredecessor");
        if(link==0){
            SCUD_PRINT_STR("exit LinkableScheduler::_unlinkPredecessor - invalid param");
            return SCUD_RC_FAIL_INVALID_PARAM;
        }
        if(link==this){
            SCUD_PRINT_STR("exit LinkableScheduler::_unlinkPredecessor - self referencing");
            return SCUD_RC_FAIL_INVALID_PARAM;
        }

        this->lockerLinkable.lock();

        this->prev=0;

        Tid linkId=link->getId();
        typename std::map<Tid,InternalContainer>::iterator icit=id2prepended.find(linkId);
        InternalContainer ic=icit->second;
        id2prepended.erase(icit);
        this->lockerLinkable.unlock();
        this->_releaseScheduledEntry(linkId,link,ic.scps.weight,ic.scps.priority);

        SCUD_PRINT_STR("exit LinkableScheduler::_unlinkPredecessor");
        return SCUD_RC_OK;
    };
    SCUD_RC _unlinkSuccessor(Linkable<TSchedulable,Tid>*  link){
        SCUD_PRINT_STR("enter LinkableScheduler::_unlinkSuccessor");
        if(link==0){
            SCUD_PRINT_STR("exit LinkableScheduler::_unlinkSuccessor");
            return SCUD_RC_FAIL_INVALID_PARAM;
        }
        if(link==this){
            SCUD_PRINT_STR("exit LinkableScheduler::_unlinkSuccessor - self referencing link");
            return SCUD_RC_FAIL_INVALID_PARAM;
        }
        this->lockerLinkable.lock();
        this->next=0;
        this->lockerLinkable.unlock();
        SCUD_PRINT_STR("exit LinkableScheduler::_unlinkSuccessor - OK");
        return SCUD_RC_OK;
    };
    SCUD_RC _pull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
        SCUD_RC retcode=SCUD_RC_FAIL_LINK_NO_PACKET_AVAILABLE;
        bool objEnded=false;
        while(retcode==SCUD_RC_FAIL_LINK_NO_PACKET_AVAILABLE){
            Linkable<TSchedulable,Tid>* link=this->calculateNextSource(objEnded);
            if(link){
                retcode=link->_pull(qu);
                if( link==0)
                    break;
            }
            objEnded=true;
        }
        
        return retcode;
    }
    void _signalAvailability(bool canPull, long long countAvailable,float weight,char priority){
        
    };
    SCUD_RC _linkPredecessor(Linkable<TSchedulable,Tid>* link){
        SCUD_PRINT_STR("enter LinkableScheduler::linkPredecessor");
        if(link==0){
            SCUD_PRINT_STR("exit LinkableScheduler::linkPredecessor");
            return SCUD_RC_FAIL_INVALID_PARAM;
        }
        if(link==this){
            SCUD_PRINT_STR("exit LinkableScheduler::linkPredecessor");
            SCUD_THROW_EXCEPTION("exit LinkableScheduler::linkPredecessor");
            return SCUD_RC_FAIL_SELF_REFERENCING_LINK_DETECTED;
        }
        if(link->hasAfter()){
            SCUD_PRINT_STR("exit LinkableScheduler::linkPredecessor");
            SCUD_THROW_EXCEPTION("exit LinkableScheduler::linkPredecessor");
            return SCUD_RC_FAIL_LINK_HAS_SUCCESSOR;
        }
        SCUD_RC res=SCUD_RC_OK;
        
        typename std::map<Tid,InternalContainer>::iterator it;
        this->lockerLinkable.lock();
        Tid linkId=link->getId();
        it = id2prepended.find(linkId);
        if (it == id2prepended.end()){
            double w=link->getWeight();
            char pr=link->getPriority();
            if(this->_scheduleEntry(linkId,link,w,pr)){
                InternalContainer ic(link,w,pr);
                id2prepended.insert(std::make_pair(linkId, ic));
                //link->_linkSuccessor(this);
                this->_scheduleFinalizeEntry(linkId,link);
                //typename Linkable<TSchedulable, Tid>::SchedulingProperties scps(w,pr);
                //this->_propagateSchedulingProperties(this,scps);
                SCUD_PRINT_STR("exit LinkableScheduler::linkPredecessor - OK");
                res= SCUD_RC_OK;
            }else{
                SCUD_PRINT_STR("exit LinkableScheduler::linkPredecessor - FAIL: could not link");
                SCUD_THROW_EXCEPTION("exit LinkableScheduler::linkPredecessor - FAIL: could not link");
                res= SCUD_RC_LINK_FAILED;
            }
        }else{
            SCUD_PRINT_STR("exit LinkableScheduler::linkPredecessor - link exists");
            res= SCUD_RC_FAIL_LINK_EXISTS;
        }
        this->lockerLinkable.unlock();
        return res;
    };

public:
    ~LinkableScheduler(){};
    LinkableScheduler(){
#ifdef SCUD_DEBUG_MODE_ENABLED
        this->elementClass="Scheduler";
#endif
    };
    SCUD_RC pull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
        SCUD_RC retcode=this->_pull(qu);
        return retcode;
    }
    SCUD_RC linkPredecessor(Linkable<TSchedulable,Tid>* link){
        SCUD_RC rc=this->_linkPredecessor(link);
        if( rc==SCUD_RC_OK){
            link->_linkSuccessor(this);
        }
        return rc;
    }
        typename Linkable<TSchedulable,Tid>::LinkedObjectsTuple unlink(SCUD_RC* rc){
        typename Linkable<TSchedulable,Tid>::LinkedObjectsTuple ptn(0,this,0);
        SCUD_PRINT_STR("enter LinkableScheduler::unlink");
        this->lockerLinkable.lock();
        for (typename std::map<Tid,InternalContainer>::iterator it=id2prepended.begin(); it!=id2prepended.end(); ++it){
            ptn.prevObject.push_back(it->second.link);
            it->second.link->_unlinkSuccessor(this);
            this->_releaseScheduledEntry(it->first,it->second.link,it->second.scps.weight,it->second.scps.priority);
        }
        id2prepended.clear();
        ptn.nextObject=this->next;
        Linkable<TSchedulable,Tid>* n=this->next;
        this->lockerLinkable.unlock();
        if(n){
            n->_unlinkPredecessor(this);
        }
        
        SCUD_PRINT_STR("exit LinkableScheduler::unlink");
        *rc=SCUD_RC_OK;
        return ptn;
        ;
    };
    bool hasBefore(){
        /*
        SCUD_PRINT_STR("enter LinkableScheduler::hasBefore");
        bool res=false;
        this->lockerSched.lock();
        if(this->id2prepended.size()>0){
            res=true;
        }
        this->lockerSched.unlock();
        SCUD_PRINT_STR("exit LinkableScheduler::hasBefore");
         */
        return false;
    }
    SCUD_RC push(TSchedulable sch, long long schedulingParam){
        return SCUD_RC_OK;
    }
    SCUD_RC pullAndPush(){
        SCUD_PRINT_STR("enter and exit LinkableScheduler::pullAndPush");
        return SCUD_RC_OK;
    };

};
    /*
     -----------------------------------------------------------------------------
     NAIVE ROUND ROBIN
     
     -----------------------------------------------------------------------------
     */
template<typename TSchedulable,typename Tid> class  LinkableSchedulerNaiveRR:public LinkableScheduler<TSchedulable,Tid>{
    protected:
    typename std::map<Tid,typename LinkableScheduler<TSchedulable,Tid>::InternalContainer>::iterator rit;
    bool _scheduleEntry(Tid linkId,Linkable<TSchedulable,Tid>* link,float weight,char priority){
        
        return true;
    };
    bool _scheduleFinalizeEntry(Tid linkId,Linkable<TSchedulable,Tid>* link){
        this->rit=this->id2prepended.begin();
        return true;
    }
    void _releaseScheduledEntry(Tid linkId,Linkable<TSchedulable,Tid>* link,float weight,char priority){
        this->rit=this->id2prepended.begin();
    }
    Linkable<TSchedulable,Tid>* calculateNextSource(bool pktsEnded){
        Linkable<TSchedulable,Tid>* l=0;
        this->lockerLinkable.lock();
        long long entriesCount=this->id2prepended.size();
        if(entriesCount==0){
            this->lockerLinkable.unlock();
            return l;
        }
        if(entriesCount==1){
            this->rit=this->id2prepended.begin();
            l=this->rit->second.link;
            if(l && l->canPull()){
                
            }else{
                this->lockerLinkable.unlock();
                return 0;
            }
        }else
        {
            long long count=0;
            while(1)
            {
                if(this->rit==this->id2prepended.end()){
                    this->rit=this->id2prepended.begin();
                }
                l=this->rit->second.link;
                ++(this->rit);
                ++count;
                if(l && l->canPull())
                    break;
                if(count>=entriesCount){
                    l=0;
                    break;
                }
            }
            if(this->rit==this->id2prepended.end()){
                this->rit=this->id2prepended.begin();
            }
        }
        this->lockerLinkable.unlock();
        return l;
    };
    public:
        LinkableSchedulerNaiveRR(Tid tid){
#ifdef SCUD_DEBUG_MODE_ENABLED
            this->elementClass="SchedulerNaiveRR";
#endif
            this->setId(tid);
            this->rit=this->id2prepended.begin();
        };
        LinkableSchedulerNaiveRR(){
#ifdef SCUD_DEBUG_MODE_ENABLED
            this->elementClass="SchedulerNaiveRR";
#endif
            this->setId(this);
            this->rit=this->id2prepended.begin();
        };
        bool canPull(){
            bool res=false;
            SCUD_PRINT_STR("enter LinkableSchedulerNaiveRR::canPull");
            this->lockerLinkable.lock();
            long long entriesCount=this->id2prepended.size();
            if(entriesCount>0){
//                Linkable<TSchedulable,Tid>* l=0;
//                l=this->rit->second.link;
//                if(l){
//                    res=l->canPull();
//                }
                res=true;
            }else{
                
            }
            this->lockerLinkable.unlock();
            SCUD_PRINT_STR("exit LinkableSchedulerNaiveRR::canPull");
            return res;
        }
    };
    /*
     -----------------------------------------------------------------------------
     PRIORITY SCHEDULER
     
     -----------------------------------------------------------------------------
     
     */
template<typename TSchedulable,typename Tid> class  LinkableSchedulerPriority:public LinkableScheduler<TSchedulable,Tid>{
    
    Linkable<TSchedulable, Tid>* prioritizedSources[SCUD_MAX_NUMBER_OF_AVAILABLE_PRIORITIES];
    void resetPrioritySlot(char prio){
        if(prio<0 || prio>=SCUD_MAX_NUMBER_OF_AVAILABLE_PRIORITIES)
            return;
        this->lockerLinkable.lock();
        prioritizedSources[prio]=0;
        if(currentMaxPriority==prio){
            currentMaxPriority=-1;
            for(int i=SCUD_MAX_NUMBER_OF_AVAILABLE_PRIORITIES;i>-1;--i){
                if(prioritizedSources[prio]){
                    currentMaxPriority=i;
                    break;
                }
            }
        }
        this->lockerLinkable.unlock();
    }
    void resetPrioritySlotByLink(Linkable<TSchedulable, Tid>* link){
        this->lockerLinkable.lock();
         for(int i=0;i<SCUD_MAX_NUMBER_OF_AVAILABLE_PRIORITIES;++i)
         {
             if(prioritizedSources[i]==link)
             {
                 prioritizedSources[i]=0;
                 break;
             }
         }
        this->lockerLinkable.unlock();
    
    }
    void resetPriorities(){
        this->lockerLinkable.lock();
        for(int i=0;i<SCUD_MAX_NUMBER_OF_AVAILABLE_PRIORITIES;++i){
            prioritizedSources[i]=0;
        }
        currentMaxPriority=-1;
        this->lockerLinkable.unlock();
    }
protected:
    char currentMaxPriority;
    long skipEntry;

    Linkable<TSchedulable,Tid>* calculateNextSource(bool objsEnded){
        this->lockerLinkable.lock();
        Linkable<TSchedulable,Tid>* l=0;
        long long entriesCount=this->id2prepended.size();
        if(entriesCount==0 || currentMaxPriority<0){
            this->lockerLinkable.unlock();
            skipEntry=0;
            return l;
        }
        l=prioritizedSources[currentMaxPriority];
        //if last high-priority source was disconnected then move max priority pointer to next available entry
        if(l==0){
            skipEntry=0;
            while (currentMaxPriority>-1 && prioritizedSources[currentMaxPriority]==0){
                l=prioritizedSources[currentMaxPriority];
                --currentMaxPriority;
            }
        }else
        //if last high-priority source temporarily has no objects - offset max priority entry
        if(objsEnded){
            if(skipEntry>currentMaxPriority){
                skipEntry=0;
            }
            ++skipEntry;
            while (currentMaxPriority-skipEntry>-1 ){
                l=prioritizedSources[currentMaxPriority-skipEntry];
                if(l)
                    break;
                ++skipEntry;
            }
        }
        this->lockerLinkable.unlock();
        return l;
    };

    void _signalAvailability(bool canPull, long long countAvailable,float weight,char priority){
        this->lockerLinkable.lock();
        skipEntry=0;
        //if objects can be pulled
        if(canPull){
            //then update current priority
            if(priority>currentMaxPriority){
                currentMaxPriority=priority;
            }
        }else{
            if(currentMaxPriority==0){
                currentMaxPriority=-1;
            }else{
                //update the current priority to the first found lower than the given one
                for(int i=priority-1;i>-1;--i){
                    if(prioritizedSources[i]){
                        currentMaxPriority=i;
                        break;
                    }
                }
            }
            
        }
        this->lockerLinkable.unlock();
    }
    void _releaseScheduledEntry(Tid linkId,Linkable<TSchedulable,Tid>* link,float weight,char priority){
        //char p=link->getPriority();
        resetPrioritySlot(priority);
    };
    bool _scheduleFinalizeEntry(Tid linkId,Linkable<TSchedulable,Tid>* link){
        return true;
    }
    bool _scheduleEntry(Tid linkId,Linkable<TSchedulable,Tid>* link,float weight,char p){
        //char p=link->getPriority();
        if(p<0){
            SCUD_PRINT_STR("exit LinkableScheduler::_scheduleEntry - priority value less than zero; NOT scheduled");
            return false;
        }
        if(prioritizedSources[p]){
            SCUD_PRINT_STR("exit LinkableScheduler::_scheduleEntry - priority value used already; NOT scheduled");
            return false;
        }
        prioritizedSources[p]=link;
        if(p>currentMaxPriority){
            currentMaxPriority=p;
        }
        return true;
    }
    SCUD_RC _propagateSchedulingProperties(Linkable<TSchedulable,Tid>* link,typename Linkable<TSchedulable,Tid>::SchedulingProperties scps){
        if(link==0){
            SCUD_PRINT_STR("exit LinkableScheduler::_propagateSchedulingProperties - invalid param");
            return SCUD_RC_FAIL_INVALID_PARAM;
        }
        this->lockerLinkable.lock();
        if(prioritizedSources[scps.priority]!=0){
            this->lockerLinkable.unlock();
            SCUD_PRINT_STR("exit LinkableScheduler::_propagateSchedulingProperties - priority value used already");
            return SCUD_RC_FAIL_INVALID_PRIORITY;
        }
        Tid linkId=link->getId();
        struct LinkableScheduler<TSchedulable,Tid>::InternalContainer ic=this->id2prepended[linkId];
        if(ic.scps.priority>-1){
            prioritizedSources[ic.scps.priority]=0;
        }
        ic.scps.priority=scps.priority;
        prioritizedSources[scps.priority]=link;
        this->id2prepended[linkId]=ic;
        if(scps.priority>currentMaxPriority){
            currentMaxPriority=scps.priority;
        }
        this->lockerLinkable.unlock();
        SCUD_PRINT_STR("exit LinkableScheduler::_propagateSchedulingProperties");
        return SCUD_RC_OK;
    };
public:
    LinkableSchedulerPriority(Tid tid){
#ifdef SCUD_DEBUG_MODE_ENABLED
        this->elementClass="SchedulerPriority";
#endif
        resetPriorities();
        this->setId(tid);
        skipEntry=0;
    };
    LinkableSchedulerPriority(){
#ifdef SCUD_DEBUG_MODE_ENABLED
        this->elementClass="SchedulerPriority";
#endif
        resetPriorities();
        this->setId(this);
        skipEntry=0;
    };
    SCUD_RC pull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
        SCUD_RC retcode= this->_pull(qu);
        return retcode;
    }

    SCUD_RC drop(){
        return SCUD_RC_OK;
    }
    bool canPull(){
        bool res=true;
        this->lockerLinkable.lock();
        if(currentMaxPriority<0||this->id2prepended.size()==0){
            res=false;
        }
        this->lockerLinkable.unlock();
        return res;
    }
};
    
    /*
     -----------------------------------------------------------------------------
     PASS-THROUGH LINKABLE OBJECT
     
     -----------------------------------------------------------------------------
     
     */
template<typename TSchedulable,typename Tid> class LinkablePass :public Linkable<TSchedulable,Tid>{
protected:
    SCUD_RC _pull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
        SCUD_RC retcode=SCUD_RC_OK;
        SCUD_PRINT_STR("enter LinkablePass::_pull");
        this->lockerLinkable.lock();
        Linkable<TSchedulable,Tid>* p=this->prev;
        this->lockerLinkable.unlock();
        if(p && _canPull())
        {
            SCUD_PRINT_STR("+LinkablePass::_pull");
            retcode=p->_pull(qu);
            SCUD_PRINT_STR("-LinkablePass::_pull");
        }else{
            retcode=SCUD_RC_FAIL_LINK_NO_PACKET_AVAILABLE;
        }
        
        SCUD_PRINT_STR("exit LinkableNull::_pull");
        return retcode;
    }
    void _signalAvailability(bool canPull, long long countAvailable, float weight,char priority){
        SCUD_PRINT_STR("LinkablePass::_signalAvailability");
        this->lockerLinkable.lock();
        this->next->_signalAvailability(canPull,countAvailable,weight,priority);
        this->lockerLinkable.unlock();
    }
    bool _canPull(){
        bool res=false;
        
        SCUD_PRINT_STR("enter LinkablePass::_canPull");
        this->lockerLinkable.lock();
        res= this->prev->canPull();
        this->lockerLinkable.unlock();
        SCUD_PRINT_STR("exit LinkablePass::_canPull");
        return res;
    }
    public:
        LinkablePass(Tid tid){
#ifdef SCUD_DEBUG_MODE_ENABLED
            this->elementClass="PassThrough";
#endif
            this->setId(tid);
        };
        LinkablePass(){
#ifdef SCUD_DEBUG_MODE_ENABLED
            this->elementClass="PassThrough";
#endif
            this->setId(this);
        }
    SCUD_RC push(TSchedulable sch, long long schedulingParam){
        SCUD_RC retcode=SCUD_RC_OK;
        this->lockerLinkable.lock();
        Linkable<TSchedulable,Tid>* n=this->next;
        this->lockerLinkable.unlock();
        if(n){
            
                retcode=n->push(sch,schedulingParam);
            
        }else{
            retcode=SCUD_RC_FAIL_LINK_NOT_EXISTS;
        }
        
        return retcode;
    }
    SCUD_RC pull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
        SCUD_RC retcode=SCUD_RC_OK;
        SCUD_PRINT_STR("enter LinkablePass::pull");
        retcode= this->_pull(qu);
        SCUD_PRINT_STR("exit LinkablePass::pull");
        return retcode;
    }
    bool canPull(){
        SCUD_PRINT_STR("enter LinkablePass::canPull");
        this->lockerLinkable.lock();
        bool res=this->_canPull();
        this->lockerLinkable.unlock();
        SCUD_PRINT_STR("exit LinkablePass::canPull");
        return res;
    }

    };
};

#endif /* Scud_h */
