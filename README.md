# CSC450MOD7 – Concurrency: Synchronized Counter Threads in C++

The portfolio milestone is a C++ program with two threads: one counts up to 20, and a second waits for it to finish and then counts down to 0. They are coordinated with a mutex and a condition variable. The repo also has thread/mutex practice programs and a small Java secure-coding example.

## Overview

This is **CSU Global CSC450, Module 7**. The repository is an Eclipse workspace folder with four projects:

| Project | Type | Contents |
|---|---|---|
| `CSC450_Mod7_Port_Milestone_Mossberg/` | Eclipse CDT (MinGW GCC) | The portfolio milestone: up and down counter threads |
| `Threads/` | Eclipse CDT | A shared counter protected by `std::lock_guard` |
| `MutexExample/` | Eclipse CDT | A commented `std::mutex` lock/unlock example (the "phone booth" analogy) |
| `CSC450_Mod7_Java/` | Eclipse Java | `LoginService`, which shows how to handle a password safely in memory |

## Portfolio milestone – `CSC450_Mod7_Port_Milestone_Mossberg.cpp`

Its shared state is in an anonymous namespace: `kLimit = 20`, a `std::mutex counterMutex`, a `std::condition_variable countReached` and a `bool upCounterDone`.

- **`countUp()`** loops from 0 to 20. On each step it takes a `std::lock_guard` and prints `count-up: n`. At 20 it sets `upCounterDone = true` (still holding the lock) and calls `countReached.notify_one()`.
- **`countDown()`** takes a `std::unique_lock` and calls **`countReached.wait(lock, [] { return upCounterDone; })`**. The predicate guards against spurious wake-ups and also covers the case where the notify happens before the wait starts. Once the flag is set it prints `count-down: 20` down to `0`.
- **`main()`** starts both threads right away and `join()`s both. The output order is always the same: up to 20 first, then down to 0.

Output (verified; one line per value):
```
count-up: 0
count-up: 1
...
count-up: 20
count-down: 20
count-down: 19
...
count-down: 0
```

## Practice programs

- **`Threads/src/Threads.cpp`**: two threads each increment a shared `int` 1,000 times. Each increment is inside a `std::lock_guard<std::mutex>` scope, so there's no data race, and the program always prints **`2000`**. Without the lock, updates could be lost.
- **`MutexExample/src/MutexExample.cpp`**: three threads call `makeACallFromPhoneBooth()`, which uses explicit `m.lock()` / `m.unlock()` around printing and incrementing a shared counter (`0 Hello Wife`, `1 Hello Wife`, `2 Hello Wife`). The comments explain the critical section as a phone booth that one person can lock at a time, and note that `std::lock_guard` would make it exception-safe.

## Java example – `CSC450_Mod7_Java/src/LoginService.java`

`authenticate(String username, char[] password)` takes the password as a **`char[]` instead of a `String`**, passes it to `checkHash`, and then **wipes it with `Arrays.fill(password, '\0')`** before returning. That keeps the secret's lifetime in memory as short as possible. `checkHash` is a stub that always returns `false`, and a comment explains that a real version would hash and compare. There's no `main`; the class only illustrates the technique.

## Build and run

C++ programs (C++11 or newer; on Linux you may need `-pthread`):

```bash
g++ -std=c++11 -pthread -o counters CSC450_Mod7_Port_Milestone_Mossberg/src/CSC450_Mod7_Port_Milestone_Mossberg.cpp && ./counters
g++ -std=c++11 -pthread -o threads Threads/src/Threads.cpp && ./threads           # 2000
g++ -std=c++11 -pthread -o mutex MutexExample/src/MutexExample.cpp && ./mutex
```

Java class (compile check only, since it has no `main`):
```bash
javac -d /tmp/out CSC450_Mod7_Java/src/LoginService.java
```

## Concepts demonstrated
- Creating `std::thread`s and joining them
- Mutual exclusion with `std::mutex`, `std::lock_guard` and `std::unique_lock`
- Ordering threads with `std::condition_variable` and a predicate wait
- Race conditions and how locks prevent them
- Secure handling of credentials in memory (`char[]` and zeroing it)
