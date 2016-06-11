/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */
/*
    This file is part of Em::Blocks.

    Copyright (c) 2011-2013 Em::Blocks

    Em::Blocks is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Em::Blocks is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Em::Blocks.  If not, see <http://www.gnu.org/licenses/>.

	@version $Revision: 4 $:
    @author  $Author: gerard $:
    @date    $Date: 2013-11-02 16:53:52 +0100 (Sat, 02 Nov 2013) $:
*/

#ifndef CBTHREADPOOL_EXTRAS_H
#define CBTHREADPOOL_EXTRAS_H

/// Josuttis' implementation of CountedPtr
template <typename T>
class CountedPtr
{
  private:
    T *ptr;
    long *count;

  public:
    explicit CountedPtr(T *p = 0);
    CountedPtr(const CountedPtr<T> &p) throw();
    ~CountedPtr() throw();
    CountedPtr<T> &operator = (const CountedPtr<T> &p) throw();
    T &operator * () const throw();
    T *operator -> () const throw();

  private:
    void dispose();
};

/** A Worker Thread class.
  *
  * These are the ones that execute the tasks.
  * You shouldn't worry about it since it's for "private" purposes of the Pool.
  */
class cbWorkerThread : public wxThread
{
  public:
    /** cbWorkerThread ctor
      *
      * @param pool Thread Pool this Worker Thread belongs to
      * @param semaphore Used to synchronise the Worker Threads
      */
    cbWorkerThread(cbThreadPool *pool, CountedPtr<wxSemaphore> &semaphore);

    /// Entry point of this thread. The magic happens here.
    ExitCode Entry();

    /// Tell the thread to abort. It will also tell the task to abort (if any)
    void Abort();

    /** Tells whether we should abort or not
      *
      * @return true if we should abort
      */
    bool Aborted() const;

    /// Aborts the running task (if any)
    void AbortTask();

  private:
    bool m_abort;
    cbThreadPool *m_pPool;
    CountedPtr<wxSemaphore> m_semaphore;
    cbThreadedTask *m_pTask;
    wxMutex m_taskMutex;
};

#endif
