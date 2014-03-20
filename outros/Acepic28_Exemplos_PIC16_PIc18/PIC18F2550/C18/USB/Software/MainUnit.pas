unit MainUnit;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtCtrls, ComCtrls, XPMan;

const
   //tamanho máximo dos buffers de envio e recepção
   BufferInSize  = 64;
   BufferOutSize = 1;

type

   //buffers de envio e recepção
   TBufferIn = array[0..BufferInSize] of byte;
   TBufferOut = array[0..BufferOutSize] of byte;

  TForm1 = class(TForm)
    Timer1: TTimer;
    Panel1: TPanel;
    Button1: TButton;
    Label3: TLabel;
    lbTensao: TLabel;
    pbTensao: TProgressBar;
    Label2: TLabel;
    lbchave: TLabel;
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure Button1Click(Sender: TObject);

    procedure Timer1Timer(Sender: TObject);
  private
    { Private declarations }
    FBufferIn:TBufferIn;
    FBufferOut:TBufferOut;
    function USBEvent(var Msg: TMessage): Boolean;
  public
    { Public declarations }
  end;

var
  Form1: TForm1;
  var tensao: real;
  var toggle: boolean = False;
  var botao: boolean = False;

implementation

uses
  USBUnit;
  //cUSBInterface, cUSBInterfaceTypes;

const

   //constantes vendorID e productID em Hexadecimal
   VENDOR_ID           = $04D8;
   PRODUCT_ID          = $003F;

{$R *.dfm}

procedure TForm1.FormCreate(Sender: TObject);
begin
  Application.HookMainWindow(USBEvent);
  Connect(Application.Handle);

end;

procedure TForm1.FormDestroy(Sender: TObject);
begin
  Application.UnHookMainWindow(USBEvent);
end;

procedure TForm1.Button1Click(Sender: TObject);
begin
botao := true;
end;

procedure TForm1.Timer1Timer(Sender: TObject);
var DevHandle  : cardinal;
begin

FBufferOut[0] := $0;

if botao = true then
   begin
   FBufferOut[1] := $80;
   botao := false;
   end
else
   begin
   if toggle = true then
      begin
      FBufferOut[1] := $37;
      toggle := false;
      end
   else
      begin
      FBufferOut[1] := $81;
      toggle := true;
      end;
   end;

DevHandle := GetHandle(VENDOR_ID, PRODUCT_ID);
Write(DevHandle,@FBufferOut);

end;

function TForm1.USBEvent(var Msg: TMessage): Boolean;
var
   DevHandle  : cardinal;

   resVlt     : integer;
   //RecStr     : String;
   //TextBuffer : Array [0..255] of char;
begin
  result := False;
  if Msg.Msg = WM_HID_EVENT then

  begin
     case Msg.WParam of

        // Evento : un dispositivo HID  insertado
        NOTIFY_PLUGGED :
        begin
           // handle del dispositivo HID
           DevHandle := Msg.LParam;
           // verificamos si se trata de nuestro dispositivo HID - USB con su VID PID
           if (GetVendorID(DevHandle) = VENDOR_ID) and (GetProductID(DevHandle) = PRODUCT_ID) then
           begin
              //Label1.Caption:= 'Dispositivo Conectado';
              Form1.Caption:= 'ACEPIC 28 - Dispositivo Conectado';
           end;
           result := true;
        end;

        // Evento : un dispositivo HID  Removido
        NOTIFY_UNPLUGGED :
        begin
           // handle del dispositivo HID
           DevHandle := Msg.LParam;
           // verificamos si se trata de nuestro dispositivo HID - USB con su VID PID
           if (GetVendorID(DevHandle) = VENDOR_ID) and (GetProductID(DevHandle) = PRODUCT_ID) then
           begin
              //Label1.Caption:= 'Dispositivo Desconectado';
              Form1.Caption:= 'ACEPIC 28 - Dispositivo Desconectado';
              pbTensao.Position := 0;
              lbTensao.Caption := '0000';
              lbchave.Caption:= 'Desconhecida.'
           end;
           result := true;
        end;

        // un dispositivo HID insertado o removido
        // este evento se dispara despues de NotifyPlugged o NotifyUnplugged.
        NOTIFY_CHANGED :
        begin
           // and set it's read notification flag to true...
           DevHandle := GetHandle(VENDOR_ID,PRODUCT_ID);
           SetReadNotify(DevHandle,true);
           result := true;
        end;

        //Um dos dispositivos HID está enviando dados
        NOTIFY_READ :
        begin
           //beep;
           DevHandle := Msg.LParam;
           //Verifica se o dispositivo enviando dado se trata do dispositivo
           //que nos interessa. Essa verificação se faz pelo VID e PID
           if (GetVendorID(DevHandle) = VENDOR_ID) and (GetProductID(DevHandle) = PRODUCT_ID) then
           begin
               //Verificado o dispositivo, será feito o armazenamento dos dados no
               //buffer FBufferIn, lembrando que o primeiro byte é o REPORT ID e
               //não faz parte do dado.
               Read(DevHandle,@FBufferIn);

               // la funcion GetOutputReportLength  obtiene la longitud(bytes) del paquete que ha llegado del puerto
               (*for index:=1 to 14 do //GetOutputReportLength(DevHandle)-1 do  //pudimos usar ...  BufferInSize
               begin

               RecStr:= RecStr + InttoStr(FBufferIn[index]) + ':';

               end;*)

               if FBufferIn[1] = $37 then
                  begin
                  resVlt := FbufferIn[3] shl 8 + FBufferIn[2];

                  tensao := (resVlt * 5000)/1023;

                  lbTensao.Caption := IntToStr(Trunc(tensao)) + ' mV';

                  pbTensao.Position := resVlt;
                  end;
                if FBufferIn[1] = $81 then
                  begin
                  if FbufferIn[2] = $00 then
                    lbChave.Caption := 'Não pressionada.'
                  else
                    lbChave.Caption := 'Pressionada.';
                  end;

               //LDataRec.Caption := RecStr;
               //RecStr:='';
           end;
           result := true;
        end;
     end;
  end;
end;

end.
