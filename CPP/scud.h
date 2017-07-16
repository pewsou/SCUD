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

#include <iostream>
#include "deque"
#include "map"
#include "vector"

namespace SCUD{

//#define SCUD_USE_EXCEPTIONS 1
#define SCUD_MAX_NUMBER_OF_AVAILABLE_PRIORITIES 64
#define SCUD_DEBUG_MODE_ENABLED
//#define SCUD_CUSTOM_MUTEX_AVAILABLE

//-------------------------------------------------------
//         PLEASE DO NOT EDIT BELOW THIS LINE
//-------------------------------------------------------
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
        long double real;
        unsigned long long unsignedInt;
        struct{
            char* ptr;
            unsigned long long size;
        } ptr;
        
    } primitives;
    _Prim(){
        retCode=SCUD_RC_OK;
    }
} Schedulable;

#ifndef SCUD_CUSTOM_MUTEX_AVAILABLE
#include "mutex"
class Locker{
    std::mutex mut;
public:
    Locker(){};
    void lock(){mut.lock();};
    void unlock(){mut.unlock();};
    virtual ~Locker(){};
};
#else
class Locker{
public:
    Locker(){};
    void lock(){};
    void unlock(){};
    virtual ~Locker(){};
};
#endif

template<typename TSchedulable,typename Tid> class LinkableDropper ;
template<typename TSchedulable,typename Tid> class LinkableQueue;
template<typename TSchedulable,typename Tid> class LinkableScheduler ;
template<typename TSchedulable,typename Tid> class LinkableSchedulerPriority;
template<typename TSchedulable,typename Tid> class LinkableNull;
    /*
     -----------------------------------------------------------------------------
     LINKABLE
     
     -----------------------------------------------------------------------------
     
     */
template<typename TSchedulable, typename Tid> class Linkable{
protected:
    long highT;
    long lowT;
    Tid itsId;
public:
    struct Queueable{
        TSchedulable scheduled;
        unsigned long long schParam;
        Queueable(){schParam=(unsigned long long)-1;};
    };
protected:
    struct SchedulingProperties{
        float weight;
        char priority;
        SchedulingProperties(){weight=-1;priority=-1;}
        SchedulingProperties(float w,char p){weight=w;priority=p;}
    };
    //bool hasPackets;
    friend class LinkableQueue<TSchedulable,Tid>;
    friend class LinkableDropper<TSchedulable,Tid>;
    friend class LinkableScheduler<TSchedulable,Tid>;
    friend class LinkableSchedulerPriority<TSchedulable,Tid>;
    friend class LinkableNull<TSchedulable,Tid>;
    Locker locker;
    Linkable<TSchedulable,Tid>* next;
    Linkable<TSchedulable,Tid>* prev;
    SchedulingProperties scp;
    Tid getId(){
        locker.lock();
        Tid t=itsId;
        locker.unlock();
        return t;
    };
    void setId(Tid tid){
        locker.lock();
        itsId=tid;
        locker.unlock();
    };
    virtual void _signalAvailability(bool canPull,unsigned long long countAvailable, float weight,char priority)=0;
    virtual SCUD_RC _pull(struct Linkable<TSchedulable,Tid>::Queueable& qu)=0;
    virtual SCUD_RC _unlinkPredecessor(Linkable<TSchedulable,Tid>*  link){
        SCUD_PRINT_STR("enter Linkable::_unlinkPredecessor");
        locker.lock();
        prev=0;
        locker.unlock();
        SCUD_PRINT_STR("exit Linkable::_unlinkPredecessor");
        return SCUD_RC_OK;
    };
    virtual SCUD_RC _unlinkSuccessor(Linkable<TSchedulable,Tid>*  link){
        SCUD_PRINT_STR("enter Linkable::_unlinkSuccessor");
        locker.lock();
        next=0;
        locker.unlock();
        SCUD_PRINT_STR("exit Linkable::_unlinkSuccessor");
        return SCUD_RC_OK;
    };
    virtual SCUD_RC _linkPredecessor(Linkable<TSchedulable,Tid>*  link){
        SCUD_PRINT_STR("enter Linkable::_linkPredecessor");
        locker.lock();
        prev=link;
        locker.unlock();
        SCUD_PRINT_STR("exit Linkable::_linkPredecessor");
        return SCUD_RC_OK;
    };
    virtual SCUD_RC _linkSuccessor(Linkable<TSchedulable,Tid>*  link){
        SCUD_PRINT_STR("enter Linkable::_linkSuccessor");
        locker.lock();
        next=link;
        locker.unlock();
        SCUD_PRINT_STR("exit Linkable::_linkSuccessor");
        return SCUD_RC_OK;
    };
    virtual SCUD_RC _propagateSchedulingProperties(Linkable<TSchedulable,Tid>*  link,SchedulingProperties scps){
        return SCUD_RC_OK;
    };

public:
    struct LinkedObjectsTuple {
        std::vector<Linkable<TSchedulable,Tid>* > prevObject;
        Linkable<TSchedulable,Tid>* thisObject;
        Linkable<TSchedulable,Tid>* nextObject;
        LinkedObjectsTuple(Linkable<TSchedulable,Tid>* p,Linkable<TSchedulable,Tid>* t,Linkable<TSchedulable,Tid>* n){
            prevObject.push_back(p);
            thisObject=t;
            nextObject=n;
        };
    } ;
    
    Linkable():lowT(0),highT(0),next(0),prev(0){
        //objects.reserve(1024);
        scp.weight=0;
        scp.priority=-1;
    };
    //TODO: Consider Removal
    Linkable(unsigned long initialObjCount):lowT(0),highT(0),next(0),prev(0){
        scp.weight=0;
        scp.priority=-1;
        //objects.reserve(initialObjCount);
    };
    virtual ~Linkable(){
        SCUD_PRINT_STR("enter Linkable::~Linkable");
        locker.lock();
        prev=0;
        next=0;
        locker.unlock();
        SCUD_PRINT_STR("exit Linkable::~Linkable");
    };
    //pull object from previous link if such exists
    virtual SCUD_RC pull(struct Linkable<TSchedulable,Tid>::Queueable& qu)=0;
    //Pull object if available from previous link if such exists and push to next link if exists
    virtual SCUD_RC pullAndPush(){
        SCUD_RC retcode=SCUD_RC_OK;
        SCUD_PRINT_STR("enter Linkable::pullAndPush");
        struct Linkable<TSchedulable,Tid>::Queueable ts;
        retcode=this->pull(ts);
        if(retcode==SCUD_RC_OK){
            this->push(ts.scheduled,ts.schParam);
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
        locker.lock();
        if(this->prev){
            res=true;
        }
        locker.unlock();
        SCUD_PRINT_STR("exit Linkable::hasBefore");
        return res;
    }
    virtual bool hasAfter(){
        SCUD_PRINT_STR("enter Linkable::hasAfter");
        bool res=false;
        locker.lock();
        if(this->next){
            res=true;
        }
        locker.unlock();
        SCUD_PRINT_STR("exit Linkable::hasAfter");
        return res;
    }
    SCUD_RC setHighThreshold(long high){
        if(high<0)
            return SCUD_RC_FAIL_INVALID_PARAM;
        locker.lock();
        if(lowT>high && high>0){
            locker.unlock();
            return SCUD_RC_FAIL_INVALID_PARAM;
        }
        highT=high;
        locker.unlock();
        return SCUD_RC_OK;
    };
    SCUD_RC setLowThreshold(long low){
        if(low<0)
            return SCUD_RC_FAIL_INVALID_PARAM;
        locker.lock();
        if(low>highT && highT>0){
            locker.unlock();
            return SCUD_RC_FAIL_INVALID_PARAM;
        }
        lowT=low;
        locker.unlock();
        return SCUD_RC_OK;
    };
    virtual SCUD_RC setWeight(float w){
        if(w<0){
            SCUD_PRINT_STR("Linkable::setWeight - Attepmt of setting invalid weight:new weight is less than 0");
            return SCUD_RC_FAIL_INVALID_WEIGHT;
        }
         //setWeight(weight);
        locker.lock();
        if(next){
            typename Linkable<TSchedulable, Tid>::SchedulingProperties scps;
            scps.weight=w;
            scps.priority=this->scp.priority;
            next->_propagateSchedulingProperties(this,this->scp);
        }
        locker.unlock();
        return SCUD_RC_OK;
    };
    virtual SCUD_RC setPriority( char prio){
        if(prio<0 || prio>=SCUD_MAX_NUMBER_OF_AVAILABLE_PRIORITIES){
            SCUD_PRINT_STR("Linkable::setPriority - Attepmt of setting invalid priority:");
            SCUD_PRINT_STR("new priority is bigger than maximum possible or is less than 0");
            SCUD_PRINT_STR_WITH_PARAM("maximum possible priority is ", SCUD_MAX_NUMBER_OF_AVAILABLE_PRIORITIES-1);
            return SCUD_RC_FAIL_INVALID_PRIORITY;
        }
        
        locker.lock();
        this->scp.priority=prio;
        if(next){
            typename Linkable<TSchedulable, Tid>::SchedulingProperties scps;
            scps.weight=this->scp.weight;
            scps.priority=this->scp.priority;
            next->_propagateSchedulingProperties(this,this->scp);
        }
        locker.unlock();
        return SCUD_RC_OK;
    };
    double getWeight(){
        locker.lock();
        double weight=this->scp.weight;
        locker.unlock();
        return weight;
    };
    char getPriority(){
        locker.lock();
         char priority=this->scp.priority;
        locker.unlock();
        return priority;
    };

    //Remove given element from chain. Rules of removal for
    //Scheduler: removes given scheduler itself and all prepended elements
    //Dropper: removes only given Dropper itself
    //Queue: removes only given Queue
    virtual LinkedObjectsTuple unlink(SCUD_RC* rc) {
        SCUD_PRINT_STR("enter Linkable::unlink");
        LinkedObjectsTuple ptn(0,this,0) ;
        locker.lock();
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
        locker.unlock();
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
            
        }else
        //if(next==0)
        {
            SCUD_PRINT_STR(" Linkable::linkSuccessor - next is empty");
            locker.lock();
            next=link;
            next->_linkPredecessor(this);
            locker.unlock();
            SCUD_PRINT_STR("exit Linkable::linkSuccessor - OK");
            return SCUD_RC_OK;
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
            locker.lock();
            prev=link;
            prev->_linkSuccessor(this);
            locker.unlock();
            SCUD_PRINT_STR("exit Linkable::linkPredecessor - OK");
            return SCUD_RC_OK;
        }
        SCUD_PRINT_STR("exit Linkable::linkPredecessor - link exists");
        SCUD_THROW_EXCEPTION("exit Linkable::linkPredecessor - link exists");
        return SCUD_RC_FAIL_LINK_EXISTS;
    };
    
    //virtual Linkable<TSchedulable,Tid>* replace(Linkable<TSchedulable,Tid>*  link)=0;
 
};
    /*
     -----------------------------------------------------------------------------
      NULL OBJECT
     
     -----------------------------------------------------------------------------
     
     */
template<typename TSchedulable,typename Tid> class LinkableNull :public Linkable<TSchedulable,Tid>{
protected:
    SCUD_RC _pull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
        SCUD_RC retcode=SCUD_RC_OK;
        SCUD_PRINT_STR("enter Linkable::_pull");
        this->locker.lock();
        if(this->prev && canPull())
        {
            SCUD_PRINT_STR("+LinkableNull::_pull");
            retcode=this->prev->_pull(qu);
            SCUD_PRINT_STR("-LinkableNull::_pull");
        }else if(retcode){
            retcode=SCUD_RC_FAIL_LINK_NO_PACKET_AVAILABLE;
        }
        this->locker.unlock();
        SCUD_PRINT_STR("exit Linkable::_pull");
        return retcode;
    }
    void _signalAvailability(bool canPull,unsigned long long countAvailable, float weight,char priority){
        SCUD_PRINT_STR("LinkableNull::_signalAvailability");
    }
public:
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
        return this->prev->canPull();
        SCUD_PRINT_STR("exit LinkableNull::canPull");
    }
};
   /*
        ---------------------------------------------------------------------------
                                DROPPER
        ---------------------------------------------------------------------------
     */
template<typename TSchedulable,typename Tid> class LinkableDropper :public Linkable<TSchedulable,Tid>{
protected:
    SCUD_RC _pull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
        SCUD_RC retcode=SCUD_RC_OK;
        this->locker.lock();
        if(this->prev && canPull())
        {
            retcode=this->prev->_pull(qu);
        }else if(retcode){
            retcode=SCUD_RC_FAIL_LINK_NO_PACKET_AVAILABLE;
        }
        this->locker.unlock();
        return retcode;
    }
    void _signalAvailability(bool canPull,unsigned long long countAvailable,float weight,char priority){
        
    }
public:
    LinkableDropper(Tid tid){
        this->setId(tid);
    };
    LinkableDropper(){
        this->setId(this);
    };
    SCUD_RC pull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
        SCUD_RC retcode= this->_pull(qu);
        return retcode;
    }
    
    SCUD_RC push(TSchedulable sch, long long schedulingParam){
        return SCUD_RC_OK;
    }
    SCUD_RC setWeight(float w){
        if(this->next!=0){
            this->next->setWeight(w);
        }
        return SCUD_RC_OK;
    };
    virtual bool canPull(){
        return this->prev->canPull();
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
    SCUD_RC _pull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
        //struct Linkable<TSchedulable,Tid>::Queueable ts;
        SCUD_RC retcode=SCUD_RC_OK;
        this->locker.lock();
        unsigned long long qs=queue.size();
        if(qs>0){
            //this->hasPackets=true;
            if(this->lowT>=0 && qs>this->lowT){
                qu=this->queue.back();
                this->queue.pop_back();
                if(qs-1<=this->lowT){
                    if(this->next){
                        this->next->_signalAvailability(false,qs-1,this->scp.weight,this->scp.priority);
                    }
                }
            }else{
                if(this->next){
                    this->next->_signalAvailability(false,qs,this->scp.weight,this->scp.priority);
                }
                //this->hasPackets=false;
               retcode=SCUD_RC_FAIL_LINK_UNDER_LOW_THRESHOLD;
            }
        }else{
            if(this->next){
                this->next->_signalAvailability(false,0,this->scp.weight,this->scp.priority);
            }
            retcode=SCUD_RC_FAIL_LINK_NO_PACKET_AVAILABLE;
        }
        this->locker.unlock();
        return retcode;
    }
    void _signalAvailability(bool canPull,unsigned long long countAvailable,float weight,char priority){
        if(this->next){
            this->next->_signalAvailability(canPull, countAvailable,this->scp.weight,this->scp.priority);
        }
    }
public:
    LinkableQueue(Tid tid){
        this->setId(tid);
    };
    LinkableQueue(){
        this->setId(this);
    };
    
    SCUD_RC pull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
        SCUD_RC retcode=SCUD_RC_OK;
        retcode= this->_pull(qu);
        return retcode;
    }
    SCUD_RC push(TSchedulable sch, long long schedulingParam){
        SCUD_RC res=SCUD_RC_OK;
        this->locker.lock();
        unsigned long long qs=queue.size();
        if(this->highT>0 && qs<this->highT){
            struct Linkable<TSchedulable,Tid>::Queueable q;
            q.scheduled=sch;
            q.schParam=schedulingParam;
            this->queue.push_front(q);
            if(this->next && qs+1>this->lowT && qs-this->lowT<2){
                this->next->_signalAvailability(true,qs+1,this->scp.weight,this->scp.priority);
            }
//            else{
//                this->next->_signalAvailability(false,qs+1);
//            }
        }else if(qs>=this->highT){
            
            res=SCUD_RC_FAIL_LINK_ABOVE_HIGH_THRESHOLD;
        }
        this->locker.unlock();
        
        return res;
    }
    virtual SCUD_RC empty(){
        this->locker.lock();
        queue.empty();
        this->locker.unlock();
        return SCUD_RC_OK;
    };
    bool canPull(){
        bool res=false;
        this->locker.lock();
        unsigned long long qs=queue.size();
        if(qs>this->lowT){
            res=true;
        }
        this->locker.unlock();
        return res;
    }
    long long size(){
        this->locker.lock();
        long long _size=queue.size();
        this->locker.unlock();
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
    virtual void _releaseScheduledEntry(Tid linkId,Linkable<TSchedulable,Tid>* link)=0;
    virtual bool _scheduleEntry(Tid linkId,Linkable<TSchedulable,Tid>* link)=0;
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
        std::cout<<"<@"<<std::endl;
        this->locker.lock();
        std::cout<<"@!@"<<std::endl;
        this->prev=0;
        std::cout<<"@1@"<<std::endl;
        Tid linkId=link->getId();
        std::cout<<"@2@"<<std::endl;
        id2prepended.erase(linkId);
        this->_releaseScheduledEntry(linkId,link);
        this->locker.unlock();
        std::cout<<"@>"<<std::endl;
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
        this->locker.lock();
        this->next=0;
        this->locker.unlock();
        SCUD_PRINT_STR("exit LinkableScheduler::_unlinkSuccessor - OK");
        return SCUD_RC_OK;
    };
    SCUD_RC _pull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
        SCUD_RC retcode=SCUD_RC_OK;
        Linkable<TSchedulable,Tid>* link=this->calculateNextSource(true);
        if(link){
            retcode=link->_pull(qu);
        }else{
            retcode=SCUD_RC_FAIL_LINK_NO_PACKET_AVAILABLE;
        }
        
        return retcode;
    }
    void _signalAvailability(bool canPull,unsigned long long countAvailable,float weight,char priority){
        
    };
public:
    ~LinkableScheduler(){};
    LinkableScheduler(){};
    SCUD_RC pull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
        SCUD_RC retcode=this->_pull(qu);
        return retcode;
    }
    SCUD_RC linkPredecessor(Linkable<TSchedulable,Tid>* link){
        SCUD_PRINT_STR("enter LinkableScheduler::linkPredecessor");
        if(link==0){
            SCUD_PRINT_STR("exit LinkableScheduler::linkPredecessor");
            return SCUD_RC_FAIL_INVALID_PARAM;
        }
        if(link==this){
            SCUD_PRINT_STR("exit LinkableScheduler::linkPredecessor");
            return SCUD_RC_FAIL_SELF_REFERENCING_LINK_DETECTED;
        }
        if(link->hasAfter()){
            SCUD_PRINT_STR("exit LinkableScheduler::linkPredecessor");
            return SCUD_RC_FAIL_LINK_HAS_SUCCESSOR;
        }
        SCUD_RC res=SCUD_RC_OK;
        
        typename std::map<Tid,InternalContainer>::iterator it;
        this->locker.lock();
        Tid linkId=link->getId();
        it = id2prepended.find(linkId);
        if (it == id2prepended.end()){
            if(this->_scheduleEntry(linkId,link)){
                double w=link->getWeight();
                char pr=link->getPriority();
                InternalContainer ic(link,w,pr);
                id2prepended.insert(std::make_pair(linkId, ic));
                link->_linkSuccessor(this);
                //typename Linkable<TSchedulable, Tid>::SchedulingProperties scps(w,pr);
                //this->_propagateSchedulingProperties(this,scps);
                SCUD_PRINT_STR("exit LinkableScheduler::linkPredecessor - OK");
                res= SCUD_RC_OK;
            }else{
                SCUD_PRINT_STR("exit LinkableScheduler::linkPredecessor - FAIL: could not link");
                res= SCUD_RC_LINK_FAILED;
            }
        }else{
            SCUD_PRINT_STR("exit LinkableScheduler::linkPredecessor - link exists");
            res= SCUD_RC_FAIL_LINK_EXISTS;
        }
        this->locker.unlock();
        return res;
    };
    typename Linkable<TSchedulable,Tid>::LinkedObjectsTuple unlink(SCUD_RC* rc){
        typename Linkable<TSchedulable,Tid>::LinkedObjectsTuple ptn(0,this,0);
        SCUD_PRINT_STR("enter LinkableScheduler::unlink");
        this->locker.lock();
        for (typename std::map<Tid,InternalContainer>::iterator it=id2prepended.begin(); it!=id2prepended.end(); ++it){
            ptn.prevObject.push_back(it->second.link);
            it->second.link->_unlinkSuccessor(this);
            this->_releaseScheduledEntry(it->first,it->second.link);
        }
        id2prepended.clear();
        ptn.nextObject=this->next;
        if(this->next){
            this->next->_unlinkPredecessor(this);
        }
        this->locker.unlock();
        SCUD_PRINT_STR("exit LinkableScheduler::unlink");
        *rc=SCUD_RC_OK;
        return ptn;
        ;
    };
    bool hasBefore(){
        SCUD_PRINT_STR("enter LinkableScheduler::hasBefore");
        bool res=false;
        this->locker.lock();
        if(this->id2prepended.size()>0){
            res=true;
        }
        this->locker.unlock();
        SCUD_PRINT_STR("exit LinkableScheduler::hasBefore");
        return res;
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
    bool _scheduleEntry(Tid linkId,Linkable<TSchedulable,Tid>* link){
        rit=this->id2prepended.begin();
        return true;
    };
    void _releaseScheduledEntry(Tid linkId,Linkable<TSchedulable,Tid>* link){
        rit=this->id2prepended.begin();
    }
    Linkable<TSchedulable,Tid>* calculateNextSource(bool pktsEnded){
        this->locker.lock();
        Linkable<TSchedulable,Tid>* l=0;
        long long entriesCount=this->id2prepended.size();
        if(entriesCount==0){
            this->locker.unlock();
            //std::cout<<"entries:0"<<std::endl;
            return l;
        }
        if(entriesCount==1){
            this->rit=this->id2prepended.begin();
            l=this->rit->second.link;
            if(l && l->canPull()){
                
            }else{
                this->locker.unlock();
                return 0;
            }
        }else
        {
            long long count=0;
            while(1){
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
        }
        this->locker.unlock();
        return l;
    };
    public:
        LinkableSchedulerNaiveRR(Tid tid){
            this->setId(tid);
            this->rit=this->id2prepended.begin();
        };
        LinkableSchedulerNaiveRR(){
            this->setId(this);
            this->rit=this->id2prepended.begin();
        };

        bool canPull(){
            bool res=false;
            SCUD_PRINT_STR("enter LinkableSchedulerNaiveRR::canPull");
            this->locker.lock();
            long long entriesCount=this->id2prepended.size();
            if(entriesCount>0){
                Linkable<TSchedulable,Tid>* l=0;
                l=this->rit->second.link;
                if(l){
                    res=l->canPull();
                }
            }else{
                
            }
            this->locker.unlock();
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
        this->locker.lock();
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
        this->locker.unlock();
    }
    void resetPrioritySlotByLink(Linkable<TSchedulable, Tid>* link){
        this->locker.lock();
         for(int i=0;i<SCUD_MAX_NUMBER_OF_AVAILABLE_PRIORITIES;++i)
         {
             if(prioritizedSources[i]==link)
             {
                 prioritizedSources[i]=0;
                 break;
             }
         }
        this->locker.unlock();
    
    }
    void resetPriorities(){
        this->locker.lock();
        for(int i=0;i<SCUD_MAX_NUMBER_OF_AVAILABLE_PRIORITIES;++i){
            prioritizedSources[i]=0;
        }
        currentMaxPriority=-1;
        this->locker.unlock();
    }
protected:
    char currentMaxPriority;
    enum DroppingPolicy{
        DROP_POLICY_FREE=0,
        DROP_POLICY_TAIL,
        DROP_POLICY_HEAD
    };
    Linkable<TSchedulable,Tid>* calculateNextSource(bool pktsEnded){
        this->locker.lock();
        Linkable<TSchedulable,Tid>* l=0;
        long long entriesCount=this->id2prepended.size();
        if(entriesCount==0 || currentMaxPriority<0){
            this->locker.unlock();
            return l;
        }
//        if(pktsEnded){
//            --currentMaxPriority;
//        }
        l=prioritizedSources[currentMaxPriority];
        if(l==0){
            while (currentMaxPriority>-1 && prioritizedSources[currentMaxPriority]==0){
                --currentMaxPriority;
                l=prioritizedSources[currentMaxPriority];
                }
            }
        this->locker.unlock();
        return l;
    };

    void _signalAvailability(bool canPull,unsigned long long countAvailable,float weight,char priority){
        this->locker.lock();
        if(canPull){
            if(priority>currentMaxPriority){
                currentMaxPriority=priority;
            }
        }else{
            if(currentMaxPriority==0){
                currentMaxPriority=-1;
            }else{
                for(int i=priority-1;i>-1;--i){
                    if(prioritizedSources[i]){
                        currentMaxPriority=i;
                        break;
                    }
                }
            }
            
        }
        this->locker.unlock();
    }
    void _releaseScheduledEntry(Tid linkId,Linkable<TSchedulable,Tid>* link){
        char p=link->getPriority();
        resetPrioritySlot(p);
    };
    bool _scheduleEntry(Tid linkId,Linkable<TSchedulable,Tid>* link){
        char p=link->getPriority();
        if(p<0){
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
        this->locker.lock();
        if(prioritizedSources[scps.priority]!=0){
            this->locker.unlock();
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
        this->locker.unlock();
        SCUD_PRINT_STR("exit LinkableScheduler::_propagateSchedulingProperties");
        return SCUD_RC_OK;
    };
public:
    LinkableSchedulerPriority(Tid tid){
        resetPriorities();
        this->setId(tid);
    };
    LinkableSchedulerPriority(){
        resetPriorities();
        this->setId(this);
    };
    SCUD_RC pull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
        SCUD_RC retcode= this->_pull(qu);
        return retcode;
    }

    SCUD_RC drop(){
        return SCUD_RC_OK;
    }
    bool canPull(){
        return true;
    }
};
}
#endif /* Scud_h */
