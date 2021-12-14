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
  public class OdExSimpleWinNetThread : IDisposable
  {
    public ManualResetEvent FinishThreadWaitHandle { get; private set; }
    private Thread m_hThread;
    private UInt32 m_threadAttributes;
    private UInt32 m_threadId = 0xFFFFFFFF;

    public OdExSimpleWinNetThread(ParameterizedThreadStart runFcn, object fcnArg, List<OdExSimpleWinNetThread> attachedThreads, UInt32 threadAttributes)
    {
      attachedThreads.Add(this);
      m_threadAttributes = threadAttributes;
      FinishThreadWaitHandle = new ManualResetEvent(false);

      m_hThread = new Thread(CreateRunFunc(runFcn, fcnArg));

      m_hThread.Priority = ThreadPriority.AboveNormal;

      m_hThread.Start();
    }

    private ThreadStart CreateRunFunc(ParameterizedThreadStart runFcn, object fcnArg)
    {
      return new ThreadStart(() =>
      {
        try
        {
          m_threadId = Globals.odGetCurrentThreadId();
          Globals.odThreadsCounter().increase(1, new uint[] { m_threadId }, m_threadAttributes);

          runFcn(fcnArg);
        }
        finally
        {
          FinishThreadWaitHandle.Set();
        }
      });
    }

    public void Dispose()
    {
      if (m_threadId != 0xFFFFFFFF)
        Globals.odThreadsCounter().decrease(1, new uint[] { m_threadId });
    }
  };

}