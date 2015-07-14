/***************************************************//**
 * @file    ActiveObject.h
 * @date    February 2015
 * @author  Ocean Optics, Inc.
 *
 * This is a class that manages its own thread of execution. You can submit messages to the queue
 * of operations and they will be executed by the thread in turn. A special "done" message terminates
 * the thread. Jobs are derived from the Message class and contain the work to be performed in their
 * Execute method. This class is used by each Spectrometer to provide e.g. asynchronous acquisition. Each Spectrometer
 * also makes this object visible to associated Sequence of acquisition. This means spectrum saving is also asynchronous
 * but acquisition and saving are queued to the same execution queue (per spectrometer/sequence) so will never overlap.
 *
 *
 * LICENSE:
 *
 * Dev Kit Copyright (C) 2015, Ocean Optics Inc
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *******************************************************/

#ifndef ACTIVE_OBJECT_H
#define ACTIVE_OBJECT_H

#include <boost/circular_buffer.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>
#include <boost/progress.hpp>
#include <boost/bind.hpp>
#include <boost/scoped_ptr.hpp>

/* A bounded buffer implementation. This class allows you to push and pop elements but the
 * push operation will block if the buffer is full and the pop operation will block
 * if the buffer is empty.
 */
template <class T>
class bounded_buffer {
public:

    typedef boost::circular_buffer<T> container_type;
    typedef typename container_type::size_type size_type;
    typedef typename container_type::value_type value_type;

    explicit bounded_buffer(size_type capacity) : m_unread(0), m_container(capacity) {}

    void push_front(const value_type& item) {
        boost::mutex::scoped_lock lock(m_mutex);
        m_not_full.wait(lock, boost::bind(&bounded_buffer<value_type>::is_not_full, this));
        m_container.push_front(item);
        ++m_unread;
        lock.unlock();
        m_not_empty.notify_one();
    }

    void pop_back(value_type* pItem) {
        boost::mutex::scoped_lock lock(m_mutex);
        m_not_empty.wait(lock, boost::bind(&bounded_buffer<value_type>::is_not_empty, this));
        *pItem = m_container[--m_unread];
        lock.unlock();
        m_not_full.notify_one();
    }

private:
    bounded_buffer(const bounded_buffer&);
    bounded_buffer& operator = (const bounded_buffer&);

    bool is_not_empty() const { return m_unread > 0; }
    bool is_not_full() const { return m_unread < m_container.capacity(); }

    size_type m_unread;
    container_type m_container;
    boost::mutex m_mutex;
    boost::condition m_not_empty;
    boost::condition m_not_full;
};

class ActiveObject {
public:
    /* We will derive "tasks" from this message class and submit the tasks to the active object queue.
    * The work to be performed is in the execute method.
    */
    class Message {
    public:
        virtual ~Message() {
        }
        virtual void Execute() {
        }
    };
 
	typedef boost::shared_ptr< Message > MessagePtr;

    /* Send a message (task)_ to the active object.
    */
    void Send(MessagePtr m);
    ActiveObject();
    ~ActiveObject();

private:
    /* Start the acitive object thread of execution
    */
    void Run();

    ActiveObject(const ActiveObject&);
    ActiveObject& operator=(const ActiveObject&);
 
    // the special message that means we are finished and can stop the thread
    MessagePtr m_done;
    // the queue of jobs. see the bounded buffer template above.
    boost::shared_ptr< bounded_buffer<MessagePtr> > m_queue;
    // the thread of execution that tasks will run on.
    boost::scoped_ptr<boost::thread> m_thread;
};
 
#endif