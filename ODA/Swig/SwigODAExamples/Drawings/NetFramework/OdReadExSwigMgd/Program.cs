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
using Teigha.Core;
using Teigha.TD;
using System.IO;

namespace OdReadExSwigMgd
{
    class Program
    {
        static Program()
        {
            OdCreationNetSwigExampleLib.Helpers.OdNetSmokeTest.Use();
        }

        static void Main(string[] args)
        {
            MemoryManager mMan = MemoryManager.GetMemoryManager();
            MemoryTransaction mStartTrans = mMan.StartTransaction();
            try
            {
                OdExHostAppServices HostApp = new OdExHostAppServices();
                Console.WriteLine("\nOdReadExSwigMgd developed using {0} ver {1}", HostApp.product(), HostApp.versionString());
                OdDbLibraryInfo libInfo = TD_Db.oddbGetLibraryInfo();
                Console.WriteLine("\nOdReadExSwigMgd developed using minor major ver {0} ", HostApp.releaseMajorMinorString());
                Console.WriteLine("\nOdDbLibraryInfo  libVer {0} buildComments {1}", libInfo.getLibVersion(), libInfo.getBuildComments());

                if (args.Length != 1)
                {
                    Console.WriteLine("\tusage: OdReadExSwigMgd filename");
                    Console.WriteLine("\nPress ENTER to continue...");
                    return;
                }
                if (!File.Exists(args[0]))
                {
                    Console.WriteLine(string.Format("File {0} does not exist", args[0]));
                    return;
                }

                OdExSystemServices systemServices = new OdExSystemServices();

                TD_Db.odInitialize(systemServices);
                Globals.odrxDynamicLinker().loadModule("TD_DynBlocks");
                Globals.odrxDynamicLinker().loadApp("DbCryptModule");

                OdCryptoServices.OdSignatureVerificationResult verRes;
                OdSignatureDescription sigDesc = new OdSignatureDescription();
                OdResult res = TD_Db.validateDrawingSignature(args[0], out verRes, sigDesc);

                try
                {
                    OdDbObjectOverrule.setIsOverruling(true);

                    OdExDbAuditInfo aiAppAudit = new OdExDbAuditInfo(System.IO.Path.GetDirectoryName(args[0]) + "AuditReport.txt");
                    aiAppAudit.setFixErrors(true);
                    aiAppAudit.setPrintDest(OdAuditInfo.PrintDest.kBoth);

                    MemoryTransaction mTr = mMan.StartTransaction();
                    OdDbDatabase pDb = HostApp.readFile(args[0], true, false, FileShareMode.kShareDenyNo, "");

                    {
                        if (pDb != null)
                        {
                            Globals.odrxDynamicLinker().loadModule("ExFieldEvaluator");
                            TD_Db.oddbEvaluateFields(pDb, (int)OdDbField.EvalContext.kOpen);

                            DbDumper dumper = new DbDumper();
                            dumper.dump(pDb);
                        }
                    }
                    mMan.StopTransaction(mTr);
                }

                catch (OdError e)
                {
                    Console.WriteLine("\nODA Drawings.NET Error: {0}", HostApp.getErrorDescription((uint)e.code()));
                }
                catch (Exception ex)
                {
                    Console.WriteLine("\nSystem Error: {0}.\nPress ENTER to continue...\n", ex.Message);
                }
            }
            finally
            {
                mMan.StopTransaction(mStartTrans);
            }

            TD_Db.odUninitialize();
            Teigha.Core.Helpers.odUninit();

        }
    }
}
