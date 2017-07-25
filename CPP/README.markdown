# Namespace

SCUD

# Available classes

**Linkable** - base class for all elements; implements chaining operations;

**SCHelper** - a helper class with some auxiliary functions;

**LinkableQueue** - dynamic FIFO container; can be assigned with weight and/or priority;

**LinkableScheduler** - base class for all scheduling elements;

**LinkableSchedulerNaiveRR** - scheduler implementing naive Round Robin scheduling strategy;can be assigned with weight and/or priority;

**LinkableSchedulerPriority** - scheduler implementing priority scheduling strategy; can be assigned with weight and/or priority;

**LinkableNull** - stub element, discarding all incoming objects;

**LinkableDropper** - element discarding incoming packet based on some criteria. Probabilistic discarding applied by default; can be assigned with weight and/or priority;

# Data types

**Linkable<typename TSchedulable,typename Tid>::Queueable** - data stucture carrying user data. Consists of two fields: 
* TSchedulable scheduled - a data to be routed; 
* long long schParam; - numerical value used for scheduling; can be set to size of "scheduled" variable, for example, sizeof(int);

**TSchedulable** - generic type for user data;

**Tid** - type of identifier that will be assigned to each element;

# API
Any element can be created using unique identifier. For example:

_LinkableQueue<int,std::string> q("$");_

That means the queueing element will hold data of type **int** with unique id "$" of type **std::string**.
On the other side, declaration _LinkableQueue<int,void*> lq_ will create element  w i t h o u t  explicit id - theat means in this case you need not to specify and manage identifiers.

**NB:** all elements that you want to have in the same routing chain must have the same type signature.

Any element has next methods:
* _e.linkSuccessor(f)_ - link two elements so that output of _e_ will be input of _f_.
* _e.linkPredecessor(f)_ - link two elements so that output of _f_ will be input of _e_.
* _e.unlink()_ - remove any preceding and succeeding link from _e_. This method returns a structure which contains vector of adresses of predecessors of given element, its own address, and its successor address.
* _e.push()_ - pass yor data object into chain.
* _e.pull()_ - extract your data object from chain.
* _e.pullAndPush()_ - extract data object from predecessor of _e_ and pass it to successor of _e_.
* _e.setPriority()_ - set priority of _e_. This parameter will be used by scheduler.
 
**NB:** Any element can have no more than 1 successor; all elements except the LinkableScheduler and its derivants may have multiple predecessors; Priority Scheduler may have any number of predecessors less than some number that is user-defined on compilation stage. Anyway for Priority Scheduler the number of priorities and therefore number of predecessors may not exceed 128. To define number of priorities alter the value of macro _SCUD_MAX_NUMBER_OF_AVAILABLE_PRIORITIES_. 0 is the lowest priority.

**Behavior**:
Queues introduce additional methods:
* _size()_ - returns number of elements in given queue
* _setLowThreshold()_ - set number of objects in queue, below which pulling will not actually take place. If _pull()_ is called and queue size is less than Low Threshold the call will do nothing.
* _setHighThreshold()_ - set number of objects in queue, above which pushing will not actually take place. If _push()_ is called and queue size is greater than High Threshold the call will do nothing.
* _empty()_ - empty the queue. Remember, the data objects DO NOT release user data. User must manage data objects himself.

**NB:** Data objects travelling inside the chain are encapsulated in special data structure. This data structure in some method calls is passed by value, so if you embed complex data type into this structure it may be copied!

**Examples**
Let us build the chaining route which consists of:
2 concurrent queues with priorities 0 and 1 accordingly:

_LinkableQueue<int,void*> queue1;_
queue1.setPriority(0);
_LinkableQueue<int,void*> queue2;_
_queue2.setPriority(1);_

Both queues are connected to a priority scheduler:
_LinkableSchedulerPriority<int, void*> scheduler;_
_scheduler.linkPredecessor(&queue1);_
_queue2.linkSuccessor(&scheduler);_

Second queue is preceded by a dropper (filter):
_LinkableDropper<int,void*> dropper;_
_dropper.linkSuccessor(&queue1);_

Now we can concurrently push the data objects:

Thread 1:

_dropper.push(some_integer_number,sizeof(int));_

Thread 2:

_queue2.push(some_integer_number,sizeof(int));_

Finally, in additional thread we will extract the data objects:

Thread3: 

_struct Linkable<int,void*>::Queueable result;_

_SCUD_RC rc=SCUD_RC_OK;_

_result=scheduler.pull(&rc);_
