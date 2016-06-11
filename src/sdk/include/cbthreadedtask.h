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

#ifndef CBTHREADEDTASK_H
#define CBTHREADEDTASK_H

/// This is what you have to use instead of wxThread to add tasks to the Thread Pool.
/// It has a reduced, but similar, interface like that of wxThread.
/// Just be sure to override Execute (like wxThread's Entry) and test every now and then
/// for TestDestroy.
class cbThreadedTask
{
  public:
    /// cbThreadedTask ctor
    cbThreadedTask();

    /// cbThreadedTask dtor
    virtual ~cbThreadedTask() = 0;

    /// This function is called to tell the task to abort (check cbThreadPool::AbortAllTasks)
    void Abort();

    /// Override this function with the task's job
    /// Return value doesn't matter
    virtual int Execute() = 0;

  protected:
    /// Be sure to call this function often. If it returns true, quit your task quickly
    bool TestDestroy() const;

    /// Same as TestDestroy()
    bool Aborted() const;

  private:
    bool m_abort;
};

/* ************************************************ */
/* **************** INLINE MEMBERS **************** */
/* ************************************************ */

inline cbThreadedTask::cbThreadedTask()
: m_abort(false)
{
  // empty
}

inline cbThreadedTask::~cbThreadedTask()
{
  // empty
}

inline bool cbThreadedTask::TestDestroy() const
{
  return m_abort;
}

inline bool cbThreadedTask::Aborted() const
{
  return m_abort;
}

inline void cbThreadedTask::Abort()
{
  m_abort = true;
}

#endif
