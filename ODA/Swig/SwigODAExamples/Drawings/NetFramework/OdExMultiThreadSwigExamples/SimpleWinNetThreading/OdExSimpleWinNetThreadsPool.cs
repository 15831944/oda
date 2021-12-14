/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2021, Open Design Alliance (the "Alliance"). 
// All rights reserved. 
// 
// This software and its documentation and related materials are owned by 
// the Alliance. The software may only be incorporated into application 
// programs owned by members of the Alliance, subject to a signed 
// Membership Agreement and Supplemental Software License Agreement with the
// Alliance. The structure and organization of this software are the valuable  
// trade secrets of the Alliance and its suppliers. The software is also 
// protected by copyright law and international treaty provisions. Application  
// programs incorporating this software must include the following statement 
// with their copyright notices:
//   
//   This application incorporates Open Design Alliance software pursuant to a license 
//   agreement with Open Design Alliance.
//   Open Design Alliance Copyright (C) 2002-2021 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Text;
using Teigha.Core;
using System.IO;
using Teigha.TD;
using System.Threading;

namespace OdExMultiThreadSwigExamples
{
  public class OdExSimpleWinNetThreadsPool
  {
    List<WaitHandle> m_runningThreads = new List<WaitHandle>();
    List<OdExSimpleWinNetThread> m_lockedThreads = new List<OdExSimpleWinNetThread>();
    ~OdExSimpleWinNetThreadsPool() { Wait(); }

    public void RunNewThread(ParameterizedThreadStart runFcn, object fcnArg, UInt32 threadAttributes)
    {
      m_runningThreads.Add((new OdExSimpleWinNetThread(runFcn, fcnArg, m_lockedThreads, threadAttributes)).FinishThreadWaitHandle);
    }

    public void Wait()
    {
      if (m_runningThreads.Count != 0)
      {
        WaitHandle.WaitAll(m_runningThreads.ToArray(), -1);
        m_lockedThreads.ForEach(x => x.Dispose());
        m_runningThreads.Clear();
        m_lockedThreads.Clear();
      }
    }
  };

}