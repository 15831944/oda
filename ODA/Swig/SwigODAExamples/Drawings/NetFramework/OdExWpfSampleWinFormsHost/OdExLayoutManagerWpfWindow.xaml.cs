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

using OdDrawingsExamplesMgdLib.ExDbLayout;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Teigha.Core;
using Teigha.TD;

namespace OdExWpfSampleWinFormsHost
{
  /// <summary>
  /// Interaction logic for OdExLayoutManagerWpfWindow.xaml
  /// </summary>
  public partial class OdExLayoutManagerWpfWindow : Window
  {
    private OdDbDatabase m_Database;

    public OdExLayoutManagerWpfWindow()
    {
      InitializeComponent();

      listBox.MouseDoubleClick += ListBox_MouseDoubleClick;
    }

    public OdExLayoutManagerWpfWindow(OdDbDatabase db) : this()
    {
      m_Database = db;

      InitListBox();
    }

    private void InitListBox()
    {
      OdStringArray layouts = new OdStringArray();
      OdDbLayoutExamples.FillLayoutList(m_Database, layouts, true, true);
      listBox.ItemsSource = layouts;
    }

    private void ListBox_MouseDoubleClick(object sender, MouseButtonEventArgs e)
    {
      if (UpdateCurrentLayout())
        DialogResult = true;
      else
        DialogResult = false;
    }

    private bool UpdateCurrentLayout()
    {
      String selectedLayout = listBox.SelectedItem as String;
      OdDbObjectId selectedLayoutId = m_Database.findLayoutNamed(selectedLayout);
      if (selectedLayoutId.IsEqual(m_Database.currentLayoutId()))
        return false;
      m_Database.setCurrentLayout(selectedLayoutId);
      return true;
    }
  }
}
