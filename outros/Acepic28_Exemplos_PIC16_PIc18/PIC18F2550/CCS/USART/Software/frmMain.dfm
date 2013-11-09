object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 510
  ClientWidth = 682
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 666
    Height = 118
    Caption = 'Configura'#231#227'o'
    TabOrder = 0
    object Label1: TLabel
      Left = 16
      Top = 35
      Width = 30
      Height = 13
      Caption = 'Porta:'
    end
    object Label2: TLabel
      Left = 128
      Top = 35
      Width = 48
      Height = 13
      Caption = 'Baudrate:'
    end
    object Label3: TLabel
      Left = 264
      Top = 35
      Width = 46
      Height = 13
      Caption = 'Paridade:'
    end
    object Label4: TLabel
      Left = 399
      Top = 35
      Width = 47
      Height = 13
      Caption = 'Data Bits:'
    end
    object Label5: TLabel
      Left = 532
      Top = 35
      Width = 46
      Height = 13
      Caption = 'Stop Bits:'
    end
    object ComComboBox1: TComComboBox
      Left = 52
      Top = 32
      Width = 61
      Height = 21
      ComPort = ComSerial
      ComProperty = cpPort
      Text = 'COM1'
      Style = csDropDownList
      ItemIndex = 1
      TabOrder = 0
    end
    object ComComboBox2: TComComboBox
      Left = 182
      Top = 32
      Width = 61
      Height = 21
      ComPort = ComSerial
      ComProperty = cpBaudRate
      Text = '9600'
      Style = csDropDownList
      ItemIndex = 7
      TabOrder = 1
    end
    object ComComboBox3: TComComboBox
      Left = 316
      Top = 32
      Width = 61
      Height = 21
      ComPort = ComSerial
      ComProperty = cpParity
      Text = 'None'
      Style = csDropDownList
      ItemIndex = 0
      TabOrder = 2
    end
    object ComComboBox4: TComComboBox
      Left = 452
      Top = 32
      Width = 57
      Height = 21
      ComPort = ComSerial
      ComProperty = cpDataBits
      Text = '8'
      Style = csDropDownList
      ItemIndex = 3
      TabOrder = 3
    end
    object ComComboBox5: TComComboBox
      Left = 584
      Top = 32
      Width = 64
      Height = 21
      ComPort = ComSerial
      ComProperty = cpStopBits
      Text = '1'
      Style = csDropDownList
      ItemIndex = 0
      TabOrder = 4
    end
    object Button2: TButton
      Left = 492
      Top = 72
      Width = 75
      Height = 25
      Caption = 'Abrir Porta'
      TabOrder = 5
      OnClick = Button2Click
    end
    object Button3: TButton
      Left = 573
      Top = 72
      Width = 75
      Height = 25
      Caption = 'Fechar Porta'
      TabOrder = 6
      OnClick = Button3Click
    end
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 132
    Width = 593
    Height = 69
    Caption = 'GroupBox2'
    TabOrder = 1
    object Label7: TLabel
      Left = 128
      Top = 27
      Width = 31
      Height = 13
      Caption = 'Label7'
    end
    object Button1: TButton
      Left = 16
      Top = 24
      Width = 75
      Height = 25
      Caption = 'Inicia'
      TabOrder = 0
      OnClick = Button1Click
    end
    object ProgressBar1: TProgressBar
      Left = 208
      Top = 24
      Width = 326
      Height = 25
      Max = 5000
      TabOrder = 1
    end
  end
  object Chart1: TChart
    Left = 8
    Top = 207
    Width = 593
    Height = 295
    Legend.Visible = False
    Title.Text.Strings = (
      'Tens'#227'o')
    BottomAxis.Automatic = False
    BottomAxis.AutomaticMaximum = False
    BottomAxis.AutomaticMinimum = False
    BottomAxis.Maximum = 200.000000000000000000
    LeftAxis.Automatic = False
    LeftAxis.AutomaticMaximum = False
    LeftAxis.AutomaticMinimum = False
    LeftAxis.Maximum = 5000.000000000000000000
    View3D = False
    TabOrder = 2
    ColorPaletteIndex = 13
    object Series1: TFastLineSeries
      Marks.Arrow.Visible = True
      Marks.Callout.Brush.Color = clBlack
      Marks.Callout.Arrow.Visible = True
      Marks.Visible = False
      SeriesColor = clRed
      LinePen.Color = clRed
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
  end
  object ComSerial: TComPort
    BaudRate = br9600
    Port = 'COM1'
    Parity.Bits = prNone
    StopBits = sbOneStopBit
    DataBits = dbEight
    Events = [evRxChar, evTxEmpty, evRxFlag, evRing, evBreak, evCTS, evDSR, evError, evRLSD, evRx80Full]
    FlowControl.OutCTSFlow = False
    FlowControl.OutDSRFlow = False
    FlowControl.ControlDTR = dtrDisable
    FlowControl.ControlRTS = rtsDisable
    FlowControl.XonXoffOut = False
    FlowControl.XonXoffIn = False
    StoredProps = [spBasic]
    TriggersOnRxChar = True
    OnRxChar = ComSerialRxChar
    Left = 632
    Top = 152
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 500
    OnTimer = Timer1Timer
    Left = 632
    Top = 200
  end
end
