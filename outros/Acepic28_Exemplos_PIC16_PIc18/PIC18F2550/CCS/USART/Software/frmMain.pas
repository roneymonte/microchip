unit frmMain;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls, CPortCtl, CPort,
  Vcl.ComCtrls, Vcl.ExtCtrls, Vcl.Samples.Gauges, VCLTee.TeEngine,
  VCLTee.Series, VCLTee.TeeProcs, VCLTee.Chart;

type
  TForm1 = class(TForm)
    ComSerial: TComPort;
    GroupBox1: TGroupBox;
    ComComboBox1: TComComboBox;
    ComComboBox2: TComComboBox;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    ComComboBox3: TComComboBox;
    Label4: TLabel;
    Label5: TLabel;
    ComComboBox4: TComComboBox;
    ComComboBox5: TComComboBox;
    Button2: TButton;
    Button3: TButton;
    Timer1: TTimer;
    GroupBox2: TGroupBox;
    Label7: TLabel;
    Button1: TButton;
    ProgressBar1: TProgressBar;
    Chart1: TChart;
    Series1: TFastLineSeries;
    procedure Button1Click(Sender: TObject);
    procedure ComSerialRxChar(Sender: TObject; Count: Integer);
    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;
  x: integer;
  y: integer;

implementation

{$R *.dfm}

procedure TForm1.Button1Click(Sender: TObject);
begin
     x:= 0;
     y:= 0;
     if Timer1.Enabled then
        begin
        Timer1.Enabled := False;
        Button1.Caption := 'Inicia';
        end
     else
        begin
        Timer1.Enabled := True;
        Button1.Caption := 'Finaliza';
        end;
end;

procedure TForm1.Button2Click(Sender: TObject);
begin
  if ComSerial.Connected then
     ComSerial.Close
  else
     begin
        ComSerial.Port := ComComboBox1.Text;
        ComSerial.BaudRate := TBaudRate(ComComboBox2.ItemIndex);
        ComSerial.Open;
     end;

end;

procedure TForm1.Button3Click(Sender: TObject);
begin
  if ComSerial.Connected then
     ComSerial.Close;
end;

procedure TForm1.ComSerialRxChar(Sender: TObject; Count: Integer);
var str: string;
    ini: integer;
begin
  ComSerial.ReadStr(str,Count);
  sleep(300);
  ini := pos('AN0', str);
  if ini > 0 then
     begin
      Label7.Caption := copy(str,ini+6,4);
      ProgressBar1.Position := StrToInt(Label7.Caption);
      x:= x+1;
      y:= StrToInt(Label7.Caption);
      Series1.AddXY(x,y,'',clRed);
     end;





end;

procedure TForm1.Timer1Timer(Sender: TObject);
var envio:string;
begin
     envio := 'AD0' + #13;
     ComSerial.WriteStr(envio);
end;

end.
