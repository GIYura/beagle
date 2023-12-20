#### Сервисы синхронизации в Linux

1. Spinlock;
2. Mutex;
3. Semaphore.

#### Spinlock

Two important things should be considered:
- when the lock is not availabe, it spins wasting processor time;
- spinlock disables kernel pre-emption + disables interrupts.

NOTE:
1. Best suited to guard concurrent access to shared resource from interrupt handlers.
2. Can't be used to protect the critical section which sleeps.

#### Mutex

When a thread tries to acquire the mutex which is already held, the thread will be put 
on to wait queue and it sleeps. The processor can execute other thread meanwhile.
When the mutex holder releases the lock, on of the tasks waiting in the wait queue
will be awakened to acquire the lock.

Mutex should not be used from interrupt context.

Mutex can be used for sleeping critical section.

#### Semaphore

