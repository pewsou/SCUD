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
On the other side, declaration _LinkableQueue<int,void*> lq0_ will create element  w i t h o u t  explicit id - theat means in this case you need not to specify and manage identifiers.

**NB:** all elements that you want to have in the same routing chain must have the same type signature.

Any element has next methods:
* _e.linkSuccessor(f)_ - link two elements so that output of _e_ will be input of _f_.
* _e.linkPredecessor(f)_ - link two elements so that output of _f_ will be input of _e_.
* _e.unlink()_ - remove any preceding and succeeding link from _e_. This method returns a structure which contains vector of adresses of predecessors of given element, its own address, and its successor address.
* _e.push()_ - pass yor data object into chain.
* _e.pull()_ - extract your data object from chain.
* _e.pullAndPush()_ - extract data object from predecessor of _e_ and pass it to successor of _e_.

**NB:** Any element can have no more than 1 successor; all elements except the LinkableScheduler and its derivants may have multiple predecessors; Priority Scheduler may have any number of predecessors less than some number that is user-defined on compilation stage. Anyway for Priority Scheduler the number of priorities and therefore number of predecessors may not exceed 128. To define number of priorities alter the value of macro _SCUD_MAX_NUMBER_OF_AVAILABLE_PRIORITIES_
