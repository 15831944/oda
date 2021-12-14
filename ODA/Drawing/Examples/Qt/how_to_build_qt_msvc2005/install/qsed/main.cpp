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
//
// qsed_main.cpp
//

#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QUuid>

int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);

  QTextStream streamErr(stderr);

  enum {
    enSt_Normal // inputFileName or -option
  , enSt_Expression
  , enSt_CommandsFromFile
  } state = enSt_Normal;
  
  QString qsInputFileName;

  QStringList lstArgs = QCoreApplication::arguments();
  lstArgs.removeFirst();
  QStringList lstExpressions; 
  
  foreach (QString qsArg, lstArgs)
  {
    switch (state)
    {
    case enSt_Normal:
      if (!qsArg.indexOf("-"))
      {
        qsArg = qsArg.mid(1);
        if (qsArg.length() == 1)
        {
          switch (qsArg.toAscii().at(0))
          {
          case 'e':
            state = enSt_Expression;
            continue;
          case 'f':
            state = enSt_CommandsFromFile;
            continue;
          }
        }
        streamErr << QObject::tr("Unsupported option \"-%1\".").arg(qsArg);
        streamErr.flush();
        return 1;
      }
      qsInputFileName = qsArg;
      break;
    case enSt_Expression:
      lstExpressions << qsArg;
      state = enSt_Normal;
      break;
    case enSt_CommandsFromFile:
      {
        QFile dataCmd(qsArg);
        if (   qsArg.isEmpty()
            || !dataCmd.exists()
            || !dataCmd.open(QIODevice::ReadOnly))
        {
          streamErr << QObject::tr(
              "File \"%1\" was not found or inaccessible"
            ).arg(qsArg);
          streamErr.flush();
          return 1;
        }

        QTextStream inputCmd(&dataCmd);
        while (!inputCmd.atEnd())
        {
          QString qsLine = inputCmd.readLine().trimmed();
          if (!qsLine.isEmpty())
            lstExpressions << qsLine;
        }
      }
      state = enSt_Normal;
      break;
    } // end switch
  } // end foreach

  if (qsInputFileName.isEmpty())
  {
    // http://en.wikipedia.org/wiki/Sed
    streamErr << QObject::tr(
      "Simple stream editor usage:\n"
      "  qsed -e \"s/oldstuff/newstuff\" inputFileName >outputFileName\n"
      "   options:\n"
      "       -e      indicates that the sed expression follows\n"
      "       -f      to read the commands from the file\n"
    );
    streamErr.flush();
    return 1;
  }

  QFile dataIn(qsInputFileName);
  if (   !dataIn.exists()
      || !dataIn.open(QIODevice::ReadOnly))
  {
    streamErr << QObject::tr(
      "File \"%1\" was not found or inaccessible"
      ).arg(qsInputFileName);
    streamErr.flush();
    return 1;
  }

  QTextStream input(&dataIn);
  QTextStream output(stdout, QIODevice::WriteOnly);

  QList<int> lstExpIndexes; 
  foreach (QString exp, lstExpressions)
  {
    lstExpIndexes << 0; 
  }
  QStringList lstCmds;
  lstCmds << "s" << "replace" 
          << "reform_guid_by" << "reformby"
          << "find" 
          << "insert_at_next_line" << "insert"
          << "off" << "on";
  bool bOutput = true;

  while (!input.atEnd())
  {
    QString qsLine = input.readLine();
    bool bOutputNext = bOutput;

    int index = 0;
    foreach (QString qsExp, lstExpressions)
    {
      int& indexExp = lstExpIndexes[index++];
      if (indexExp >= qsExp.length())
        indexExp = 0;
      qsExp = qsExp.mid(indexExp);

      for (bool bRes = true; bRes;)
      {
        bRes = false;
        while (!qsExp.indexOf(" "))
        {
          qsExp = qsExp.mid(1);
          indexExp++;
        }
        if (qsExp.isEmpty())
          continue;

        int indexExpNext = indexExp;

        int indexCmd = -1;
        foreach (QString qsCmd, lstCmds)
        {
          indexCmd++;
          if (qsExp.indexOf(qsCmd))
            continue;
          qsExp = qsExp.mid(qsCmd.length());
          indexExpNext += qsCmd.length();
          switch (indexCmd)
          {
          case 0: case 1: // "s" "replace"
            {
              if (qsExp.isEmpty())
              {
                streamErr << QObject::tr("Invalid argument of replace command");
                streamErr.flush();
                return 1;
              }

              QString qsDelim = qsExp[0];
              qsExp = qsExp.mid(1);
              indexExpNext++;
              int pos = qsExp.indexOf(qsDelim);
              if (pos >= 0)
              {
                QString qsFrom = qsExp.left(pos);
                qsExp = qsExp.mid(++pos);
                indexExpNext += pos;
                pos = qsExp.indexOf(qsDelim);
                if (pos >= 0)
                {
                  QString qsTo = qsExp.left(pos);
                  qsExp = qsExp.mid(++pos);
                  indexExpNext += pos;

                  if (!qsFrom.isEmpty())
                  {
                    bRes = (qsLine.indexOf(qsFrom) >= 0);
                    qsLine.replace(qsFrom, qsTo);
                  }
                }
              }
            }
            break;
          case 2: case 3: // "reform_guid_by" "reformby"
            {
              if (qsExp.isEmpty())
              {
                streamErr << QObject::tr("Invalid argument of reformby command");
                streamErr.flush();
                return 1;
              }

              QString qsDelim = qsExp[0];
              qsExp = qsExp.mid(1);
              indexExpNext++;
              int pos = qsExp.indexOf(qsDelim);
              if (pos >= 0)
              {
                QString qsFrom = qsExp.left(pos);
                qsExp = qsExp.mid(++pos);
                indexExpNext += pos;

                if (qsFrom.isEmpty())
                {
                  streamErr << QObject::tr("An Empty argument of reformby command");
                  streamErr.flush();
                  return 1;
                }

                QUuid iid(qsFrom);
                if (iid.isNull())
                {
                  streamErr << QObject::tr("Invalid input GUID of reformby command");
                  streamErr.flush();
                  return 1;
                }

                pos = qsExp.indexOf(qsDelim);
                if (pos >= 0)
                {
                  QString qsBy = qsExp.left(pos);
                  qsExp = qsExp.mid(++pos);
                  indexExpNext += pos;

                  if (qsBy.isEmpty())
                  {
                    streamErr << QObject::tr("An empty \"by\" argument of reformby command");
                    streamErr.flush();
                    return 1;
                  }

                  //// via data4[0]
                  //if (iid.variant() != QUuid::DCE) // strange but it DCE by Qt // QUuid::Microsoft)
                  //{
                  //  ODA_FAIL_ONCE(); 
                  //}
                  //// via data3
                  //if (iid.version() != QUuid::Random)
                  //{
                  //  ODA_FAIL_ONCE(); 
                  //}

                  uchar* pData = &iid.data4[1];
                  const uchar* pBy = (const uchar*) qsBy.toAscii().data();
                  int index = 0;
                  while (*pBy)
                  {
                    pData[index] += *pBy++;
                    if (++index >= 7)
                      index = 0;
                  }

                  QString qsTo = iid.toString().toUpper();

                  //if (!qsFrom.isEmpty())
                  //{
                    bRes = (qsLine.indexOf(qsFrom) >= 0);
                    qsLine.replace(qsFrom, qsTo);
                  //}
                }
              }
            }
            break;
          case 4: // "find"
            {
              if (qsExp.isEmpty())
              {
                streamErr << QObject::tr("Invalid argument of find command");
                streamErr.flush();
                return 1;
              }

              QString qsDelim = qsExp[0];
              qsExp = qsExp.mid(1);
              indexExpNext++;
              int pos = qsExp.indexOf(qsDelim);
              if (pos >= 0)
              {
                QString qsStr = qsExp.left(pos);
                qsExp = qsExp.mid(++pos);
                indexExpNext += pos;
                if (!qsStr.isEmpty() && qsLine.indexOf(qsStr) >= 0)
                  bRes = true;
              }
            }
            break;
          case 5: case 6: // "insert_at_next_line" "insert"
            {
              if (qsExp.isEmpty())
              {
                streamErr << QObject::tr("Invalid argument of insert command");
                streamErr.flush();
                return 1;
              }

              QString qsDelim = qsExp[0];
              qsExp = qsExp.mid(1);
              indexExpNext++;
              int pos = qsExp.indexOf(qsDelim);
              if (pos >= 0)
              {
                QString qsStr = qsExp.left(pos);
                qsExp = qsExp.mid(++pos);
                indexExpNext += pos;
                output << qsLine << "\n";
                qsLine = qsStr;
              }
              bRes = true;
            }
            break;
          case 7: // "off" 
            bOutput = bOutputNext = false;
            bRes = true;
            break;
          case 8: // "on"
            bOutputNext = true;
            bRes = true;
            break;
          default:
            break;
          } // end switch

          break;

        } // end foreach

        if (bRes) 
          indexExp = indexExpNext;

      } // end for

    } // end foreach

    if (bOutput)
    {
      if (qsLine.trimmed().isEmpty())
        qsLine = "";
      output << qsLine << "\n";
    }
    bOutput = bOutputNext;
  }

  return 0;
}

