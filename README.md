*This project has been created as part of the 42 curriculum by vslyunko*

<p align="center">
  <img src="codexion-banner.png" alt="Codexion banner" width="100%">
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Language-C-245C45?style=flat&logoColor=white" alt="C">
  <img src="https://img.shields.io/badge/Concurrency-POSIX%20Threads-66785F?style=flat" alt="POSIX Threads">
  <img src="https://img.shields.io/badge/Synchronization-Mutexes%20%26%20CondVars-7D8D70?style=flat" alt="Mutexes and Condition Variables">
  <img src="https://img.shields.io/badge/Scheduling-FIFO%20%26%20EDF-445C4E?style=flat" alt="FIFO and EDF Scheduling">
</p>

<p align="center">
  A concurrent system simulation written in C 
</p>

## DESCRIPTION

Codexion is a C concurrency simulation where multiple coders compete for shared USB dongles. Using POSIX threads, mutexes, and FIFO/EDF scheduling, the program coordinates access to resources while avoiding deadlocks, starvation, and burnout.
??
Codexion is a concurrency project in C where several coders run at the same time and compete for a limited number of USB dongles. The goal is to manage those shared resources safely and fairly using threads, mutexes, and FIFO/EDF scheduling, while preventing deadlocks, starvation, and burnout.

---

## INSTRUCTIONS

**Document prerequisites:** a C compiler, Make, POSIX threads, and optionally Valgrind on the Linux testing machine.


### Compilation

```bash
make
```

Other available rules:

```bash
make run        # Stat simulation using default ARGS
make clean      # Remove object files
make fclean     # Remove object files and executable
make re         # Recompile everything
make valgrind   # Detect memory errors and leaks
make helgrind   # Checks threading problems
```

### Running the proyet

```bash
./codexion <coders> <burnout> <compile> <debug> <refactor>
    <required_compiles> <cooldown> <fifo|edf>
```
All time values are in milliseconds. Numeric arguments must be integers
between 0 and 2147483647, except `number_of_coders`, which must be positive. Scheduler names must be lowercase.


Default arguments: `5 10000 200 200 200 3 50 fifo`.
Override them for any command with `ARGS`:

```bash
make run ARGS="5 10000 200 200 200 3 50 edf"
```

**Each activity message contains:**

```text
<elapsed_time_ms> <coder_id> <activity>
```

The checks require Valgrind on Linux. They slow execution, so you may
need to increase `time_to_burnout`.

---

## RESOURCES
general idea about philosopgers: https://medium.com/@ruinadd/philosophers-42-guide-the-dining-philosophers-problem-893a24bc0fe2
14.09.26: to understand threads - https://www.youtube.com/watch?v=LOfGJcVnvAk
14.09.26: threads code example - https://www.youtube.com/watch?v=ldJ8WGZVXZk

14.09.26: threads in c list video (main concepts) - https://www.youtube.com/watch?v=d9s_d28yJq0&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2

Required Functions and Their Roles - https://studylib.net/doc/27904987/philosophers-guide

### AI usage


### Blocking cases handled

### Thread synchronization mechanisms