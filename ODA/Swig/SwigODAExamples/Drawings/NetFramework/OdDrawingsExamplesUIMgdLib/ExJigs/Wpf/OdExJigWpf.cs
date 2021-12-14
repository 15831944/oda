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


using OdDrawingsExamplesMgdLib.ExJigs;
using OdDrawingsExamplesMgdLib.ExTrackers;
using OdKernelExamplesMgdLib;
using OdKernelExamplesMgdLib.ExJigs;
using OdKernelExamplesUIMgdLib.Helpers;
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Threading;
using Teigha.Core;

namespace OdDrawingsExamplesUIMgdLib.ExJigs.Wpf
{
  public abstract class OdExJigWpf
  {
    protected UserControl m_targetControl;
    protected OdExMouseDragerBase m_jig;

    public OdExJigWpf(UserControl targetControl, OdExMouseDragerBase jig)
    {
      m_targetControl = targetControl;
      m_jig = jig;
      setupJig();
    }

    public OdExMouseDragerBase Jig { get { return m_jig; } }

    protected virtual void setupJig()
    {
      if (m_jig == null)
        throw new Exception("variable m_jig not initialized");

      m_jig.InitDragerEvent += Init;
      m_jig.UnInitDragerEvent += UnInit;
      m_jig.ApplicationDoProcessingEvent += ApplicationDoProcessingHandler;
    }

    protected abstract void Init();
    protected abstract void UnInit();

    protected virtual void ApplicationDoProcessingHandler()
    {
      //Application do events
      Application.Current.Dispatcher.Invoke(DispatcherPriority.Background, new Action(delegate { }));
    }

    protected virtual void OnMouseDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
    {
      Point pos = e.GetPosition(m_targetControl);
      m_jig.OnMouseDown(sender, OdExWpfMouseHelper.ConvertMouseButtons(e.ChangedButton), (int)pos.X, (int)pos.Y);
    }

    protected virtual void OnMouseMove(object sender, System.Windows.Input.MouseEventArgs e)
    {
      Point pos = e.GetPosition(m_targetControl);
      m_jig.OnMouseMove(sender, OdExWpfMouseHelper.ConvertMouseButtons(e), (int)pos.X, (int)pos.Y);
    }

    protected virtual void OnKeyPress(object sender, System.Windows.Input.KeyEventArgs e)
    {
      OdExKeyboardData keyData;
      KeyToCharHelper(e.Key, out keyData);

      m_jig.OnKeyPress(sender, keyData.m_character);
    }

    private class OdExKeyboardData
    {
      public Key m_key { get; set; }
      public bool m_printable { get; set; }
      public char m_character { get; set; }
      public bool m_shift { get; set; }
      public bool m_ctrl { get; set; }
      public bool m_alt { get; set; }
      public int m_type { get; set; } //sideband
    };

    private void KeyToCharHelper(Key key, out OdExKeyboardData KeyDecode)
    {
      bool iscap;
      bool caplock;
      bool shift;

      KeyDecode = new OdExKeyboardData();

      KeyDecode.m_key = key;

      KeyDecode.m_alt = Keyboard.IsKeyDown(Key.LeftAlt) ||
                        Keyboard.IsKeyDown(Key.RightAlt);

      KeyDecode.m_ctrl = Keyboard.IsKeyDown(Key.LeftCtrl) ||
                        Keyboard.IsKeyDown(Key.RightCtrl);

      KeyDecode.m_shift = Keyboard.IsKeyDown(Key.LeftShift) ||
                        Keyboard.IsKeyDown(Key.RightShift);

      if (KeyDecode.m_alt || KeyDecode.m_ctrl)
      {
        KeyDecode.m_printable = false;
        KeyDecode.m_type = 1;
      }
      else
      {
        KeyDecode.m_printable = true;
        KeyDecode.m_type = 0;
      }

      shift = KeyDecode.m_shift;
      caplock = Console.CapsLock; //Keyboard.IsKeyToggled(Key.CapsLock);
      iscap = (caplock && !shift) || (!caplock && shift);

      switch (key)
      {
        case Key.Enter: KeyDecode.m_character = '\n'; return;
        case Key.A: KeyDecode.m_character = (iscap ? 'A' : 'a'); return;
        case Key.B: KeyDecode.m_character = (iscap ? 'B' : 'b'); return;
        case Key.C: KeyDecode.m_character = (iscap ? 'C' : 'c'); return;
        case Key.D: KeyDecode.m_character = (iscap ? 'D' : 'd'); return;
        case Key.E: KeyDecode.m_character = (iscap ? 'E' : 'e'); return;
        case Key.F: KeyDecode.m_character = (iscap ? 'F' : 'f'); return;
        case Key.G: KeyDecode.m_character = (iscap ? 'G' : 'g'); return;
        case Key.H: KeyDecode.m_character = (iscap ? 'H' : 'h'); return;
        case Key.I: KeyDecode.m_character = (iscap ? 'I' : 'i'); return;
        case Key.J: KeyDecode.m_character = (iscap ? 'J' : 'j'); return;
        case Key.K: KeyDecode.m_character = (iscap ? 'K' : 'k'); return;
        case Key.L: KeyDecode.m_character = (iscap ? 'L' : 'l'); return;
        case Key.M: KeyDecode.m_character = (iscap ? 'M' : 'm'); return;
        case Key.N: KeyDecode.m_character = (iscap ? 'N' : 'n'); return;
        case Key.O: KeyDecode.m_character = (iscap ? 'O' : 'o'); return;
        case Key.P: KeyDecode.m_character = (iscap ? 'P' : 'p'); return;
        case Key.Q: KeyDecode.m_character = (iscap ? 'Q' : 'q'); return;
        case Key.R: KeyDecode.m_character = (iscap ? 'R' : 'r'); return;
        case Key.S: KeyDecode.m_character = (iscap ? 'S' : 's'); return;
        case Key.T: KeyDecode.m_character = (iscap ? 'T' : 't'); return;
        case Key.U: KeyDecode.m_character = (iscap ? 'U' : 'u'); return;
        case Key.V: KeyDecode.m_character = (iscap ? 'V' : 'v'); return;
        case Key.W: KeyDecode.m_character = (iscap ? 'W' : 'w'); return;
        case Key.X: KeyDecode.m_character = (iscap ? 'X' : 'x'); return;
        case Key.Y: KeyDecode.m_character = (iscap ? 'Y' : 'y'); return;
        case Key.Z: KeyDecode.m_character = (iscap ? 'Z' : 'z'); return;
        case Key.D0: KeyDecode.m_character = (shift ? ')' : '0'); return;
        case Key.D1: KeyDecode.m_character = (shift ? '!' : '1'); return;
        case Key.D2: KeyDecode.m_character = (shift ? '@' : '2'); return;
        case Key.D3: KeyDecode.m_character = (shift ? '#' : '3'); return;
        case Key.D4: KeyDecode.m_character = (shift ? '$' : '4'); return;
        case Key.D5: KeyDecode.m_character = (shift ? '%' : '5'); return;
        case Key.D6: KeyDecode.m_character = (shift ? '^' : '6'); return;
        case Key.D7: KeyDecode.m_character = (shift ? '&' : '7'); return;
        case Key.D8: KeyDecode.m_character = (shift ? '*' : '8'); return;
        case Key.D9: KeyDecode.m_character = (shift ? '(' : '9'); return;
        case Key.OemPlus: KeyDecode.m_character = (shift ? '+' : '='); return;
        case Key.OemMinus: KeyDecode.m_character = (shift ? '_' : '-'); return;
        case Key.OemQuestion: KeyDecode.m_character = (shift ? '?' : '/'); return;
        case Key.OemComma: KeyDecode.m_character = (shift ? '<' : ','); return;
        case Key.OemPeriod: KeyDecode.m_character = (shift ? '>' : '.'); return;
        case Key.OemOpenBrackets: KeyDecode.m_character = (shift ? '{' : '['); return;
        case Key.OemQuotes: KeyDecode.m_character = (shift ? '"' : '\''); return;
        case Key.Oem1: KeyDecode.m_character = (shift ? ':' : ';'); return;
        case Key.Oem3: KeyDecode.m_character = (shift ? '~' : '`'); return;
        case Key.Oem5: KeyDecode.m_character = (shift ? '|' : '\\'); return;
        case Key.Oem6: KeyDecode.m_character = (shift ? '}' : ']'); return;
        case Key.Tab: KeyDecode.m_character = '\t'; return;
        case Key.Space: KeyDecode.m_character = ' '; return;

        // Number Pad
        case Key.NumPad0: KeyDecode.m_character = '0'; return;
        case Key.NumPad1: KeyDecode.m_character = '1'; return;
        case Key.NumPad2: KeyDecode.m_character = '2'; return;
        case Key.NumPad3: KeyDecode.m_character = '3'; return;
        case Key.NumPad4: KeyDecode.m_character = '4'; return;
        case Key.NumPad5: KeyDecode.m_character = '5'; return;
        case Key.NumPad6: KeyDecode.m_character = '6'; return;
        case Key.NumPad7: KeyDecode.m_character = '7'; return;
        case Key.NumPad8: KeyDecode.m_character = '8'; return;
        case Key.NumPad9: KeyDecode.m_character = '9'; return;
        case Key.Subtract: KeyDecode.m_character = '-'; return;
        case Key.Add: KeyDecode.m_character = '+'; return;
        case Key.Decimal: KeyDecode.m_character = '.'; return;
        case Key.Divide: KeyDecode.m_character = '/'; return;
        case Key.Multiply: KeyDecode.m_character = '*'; return;

        default:
          KeyDecode.m_type = 1;
          KeyDecode.m_printable = false;
          KeyDecode.m_character = '\x00';
          return;
      }
    }
  }

}
