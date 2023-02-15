#ifndef SQUEUE_H
#define SQUEUE_H

/* CASPP SBUF c++ version */

#include <semaphore.h>
#include <cstdlib>
#include <string>

#include "unix_wrap.h"

template<class T>
class SyncQueue{
public:
    SyncQueue(const char *name, const size_t buf_size);
    ~SyncQueue();
    void enqueue(const T &item);
    T dequeue();

private:
    SyncQueue(const SyncQueue &);                 /* Uncopable */
    SyncQueue &operator=(const SyncQueue &);      /* Uncopable */

    std::string name;                       /* Semaphore name */
    T *buf;                                 /* Buffer array */
    int n;                                  /* Maximum number of slots */
    int front;                              /* buf[(front + 1)%n] is first item */
    int rear;                               /* buf[rear % n] is last item */
    sem_t *mutex;                           /* Protects accesses to buf */
    sem_t *slots;                           /* Counts available slots */
    sem_t *items;                           /* Counts available items */
};

template<class T>
SyncQueue<T>::SyncQueue(const char *__name, const size_t __size): n(__size), front(0), rear(0), name(__name)
{
    std::string name_buf(__name);
    name_buf.push_back('_');
    name_buf.push_back('1');

    mutex = Sem_open(name_buf.c_str(), O_CREAT, S_IRUSR|S_IWUSR, 1);

    name_buf[(name_buf.size() - 1)] = '2';
    slots = Sem_open(name_buf.c_str(), O_CREAT, S_IRUSR|S_IWUSR, __size);

    name_buf[(name_buf.size() - 1)] = '3';
    items = Sem_open(name_buf.c_str(), O_CREAT, S_IRUSR|S_IWUSR, 0);

    buf = (T *)Calloc(__size, sizeof(T));
}

template<class T>
SyncQueue<T>::~SyncQueue(){
    std::string name_buf(name);
    name_buf.push_back('_');
    name_buf.push_back('1');

    Sem_close(mutex);
    Sem_unlink(name_buf.c_str());

    name_buf[(name_buf.size() - 1)] = '2';
    Sem_close(slots);
    Sem_unlink(name_buf.c_str());

    name_buf[(name_buf.size() - 1)] = '3';
    Sem_close(items);
    Sem_unlink(name_buf.c_str());

    free(buf);
}

template<class T>
void SyncQueue<T>::enqueue(const T &item)
{
    P(slots);
    P(mutex);
    *(buf + (rear++ % n)) = item;
    V(mutex);
    V(items);
}

template<class T>
T SyncQueue<T>::dequeue()
{
    T item; 
    P(items);
    P(mutex);
    item = *(buf + (front++ % n));
    V(mutex);
    V(slots);
    return item;
}
#endif