*This project has been created as part of the 42 curriculum by <login>.*

# Philosophers

## Description

Philosophers is an implementation of the classic **dining philosophers problem**, a synchronization exercise introduced by Edsger Dijkstra.

N philosophers sit around a round table with a bowl of spaghetti in the middle. There are as many forks as philosophers, one placed between each pair. To eat, a philosopher must hold **both** the fork on their left and the fork on their right. Each philosopher cycles through three states: **eating**, **sleeping**, and **thinking**. If a philosopher goes too long without starting a meal, they die of starvation and the simulation stops.

The goal is to learn the basics of multithreading in C:

- creating and joining threads with `pthread`,
- protecting shared data with **mutexes**,
- avoiding **data races** and **deadlocks**,
- handling precise timing in milliseconds.

### Implementation overview

- Each philosopher is a **thread**.
- Each fork is a **mutex**, so its state is protected.
- A dedicated **monitor** (the main thread) continuously checks whether a philosopher has starved or whether everyone has eaten enough.
- Mutexes protect the `last meal` timestamp, the meal counter, the end-of-simulation flag, and the output (so messages never overlap).
- Deadlock is avoided by making philosophers pick up their forks in a deterministic order depending on their id, and by slightly delaying even-numbered philosophers at startup.
- No global variables are used; all shared state lives in a `t_data` structure.

## Instructions

### Compilation

```bash
make
```

Other rules: `make clean`, `make fclean`, `make re`.

### Execution

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

| Argument | Description |
|---|---|
| `number_of_philosophers` | Number of philosophers (and forks) |
| `time_to_die` (ms) | Max time without starting a meal before dying |
| `time_to_eat` (ms) | Duration of a meal (two forks held) |
| `time_to_sleep` (ms) | Duration of sleep |
| `number_of_times_each_philosopher_must_eat` | *(optional)* Simulation stops once everyone has eaten at least this many times |

### Examples

```bash
./philo 1 800 200 200        # a lone philosopher dies (only one fork)
./philo 5 800 200 200        # nobody should die
./philo 4 310 200 100        # a philosopher should die
./philo 5 800 200 200 7      # stops after each philosopher ate 7 times
```

### Output format

```
timestamp_in_ms X has taken a fork
timestamp_in_ms X is eating
timestamp_in_ms X is sleeping
timestamp_in_ms X is thinking
timestamp_in_ms X died
```

### Testing for data races

```bash
cc -Wall -Wextra -Werror -pthread -fsanitize=thread *.c -o philo_tsan
valgrind --tool=helgrind ./philo 5 800 200 200 3
```

## Resources

### Documentation and references

- `man pthread_create`, `man pthread_join`, `man pthread_mutex_lock`, `man gettimeofday`, `man usleep`
- [Dining philosophers problem (Wikipedia)](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [POSIX Threads Programming (LLNL tutorial)](https://hpc-tutorials.llnl.gov/posix/)
- Operating Systems: Three Easy Pieces, chapters on concurrency and locks

### Use of AI

AI was used as a **learning and review assistant**, not as a code generator to copy blindly. It was used to:

- explain threading concepts (mutexes, data races, deadlocks) and the reasoning behind each design choice,
- help structure the project into steps (parsing, initialization, routine, monitor, cleanup),
- review code for possible data races and edge cases.

All code was written, tested and understood by the author, and can be explained in detail during the evaluation.
