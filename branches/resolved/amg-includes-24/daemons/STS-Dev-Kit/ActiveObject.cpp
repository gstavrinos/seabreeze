/***************************************************//**
 * @file    ActiveObject.cpp
 * @date    February 2015
 * @author  Ocean Optics, Inc.
 *
 * An active object is an object that manages its own thread of execution. Tasks
 * can be submitted for execution o this thread by deriving a "message" from the
 * ActiveObject::Message class with an Execute method containing the work to be done.
 * Tasks are queued to a "bounded buffer" and executed in turn. When we are finished
 * with the active object the destructor queues a special message to end the thread
 * enabling a clean exit.
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

#include "ActiveObject.h"

/* Start the queue of tasks in a separate thread. Take jobs
* from the queue and execute them until we receive the special "done" job.
*/
void ActiveObject::Run() {
    // This method is run in a thread on construction. It takes
    // jobs from the queue and runs their Execute method
    // until the done message is sent by the destructor.
    MessagePtr msg;
    do {
        m_queue->pop_back(&msg);
        msg->Execute();
    } while (msg != m_done);
}

/* Add a task to the queue.
*/
void ActiveObject::Send(MessagePtr m) {
    m_queue->push_front(m);
}

/* Constructor. Initialises the bounded buffer (the size, 128 is arbitrary) and
* start the Run method in a separate thread.
*/
ActiveObject::ActiveObject() :
    m_done(new Message) {

    // Set up the queue of jobs and start the Run method in a thread.
    m_queue.reset(new bounded_buffer< MessagePtr >(128));
	m_thread.reset(new boost::thread(&ActiveObject::Run, this));
}

/* Desrtuctor. Send the "done" message to the queue and wait for the thread to complete.
* Note/warning: this means any remaining jobs in the queue will be completed before we are allowed to exit
* the destructor.
*/
ActiveObject::~ActiveObject() {
    // Send the done message and wait for the thread to complete
    Send(m_done);
    m_thread->join();
}
