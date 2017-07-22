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
* unsigned long long schParam; - numerical value used for scheduling; can be set to size of "scheduled" variable, for example, sizeof(int);

**TSchedulable** - generic type for user data;

**Tid** - type of identifier that will be assigned to each element;
