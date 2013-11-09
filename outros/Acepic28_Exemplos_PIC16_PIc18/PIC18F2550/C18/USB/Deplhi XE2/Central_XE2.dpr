program Central_XE2;

uses
  Forms,
  MainUnit in 'MainUnit.pas' {Form1},
  USBUnit in 'USBUnit.pas';

{$R *.res}

begin
  Application.Initialize;
  //Application.MainFormOnTaskbar := True;
  Application.Title := 'ACPC28_USB';
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
