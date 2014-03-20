unit cUSBInterface;

interface

uses
   Windows,Messages;

const
   // the plugin manager DLL name...
   DLL_NAME                     = 'mcHID.dll';

// DLL interface functions...
function Connect(pHostWin:HWND):BOOL;stdcall;external DLL_NAME name 'Connect';
function Disconnect:BOOL;stdcall;external DLL_NAME name 'Disconnect';
function GetItem(pIndex:UINT):UINT;stdcall;external DLL_NAME name 'GetItem';
function GetItemCount:UINT;stdcall;external DLL_NAME name 'GetItemCount';
function Read(pHandle:UINT;pData:pointer):BOOL;stdcall;external DLL_NAME name 'Read';
function Write(pHandle:UINT;pData:pointer):BOOL;stdcall;external DLL_NAME name 'Write';
function ReadEx(pVendorID,pProductID:UINT;pData:pointer):BOOL;stdcall;external DLL_NAME name 'ReadEx';
function WriteEx(pVendorID,pProductID:UINT;pData:pointer):BOOL;stdcall;external DLL_NAME name 'WriteEx';

function GetHandle(pVendorID,pProductID:UINT):UINT;stdcall;external DLL_NAME name 'GetHandle';
function GetVendorID(pHandle:UINT):UINT;stdcall;external DLL_NAME name 'GetVendorID';
function GetProductID(pHandle:UINT):UINT;stdcall;external DLL_NAME name 'GetProductID';
function GetVersion(pHandle:UINT):UINT;stdcall;external DLL_NAME name 'GetVersion';
function GetVendorName(pHandle:UINT;pText:LPSTR;pLen:UINT):UINT;stdcall;external DLL_NAME name 'GetVendorName';
function GetProductName(pHandle:UINT;pText:LPSTR;pLen:UINT):UINT;stdcall;external DLL_NAME name 'GetProductName';
function GetSerialNumber(pHandle:UINT;pText:LPSTR;pLen:UINT):UINT;stdcall;external DLL_NAME name 'GetSerialNumber';
function GetInputReportLength(pHandle:UINT):UINT;stdcall;external DLL_NAME name 'GetInputReportLength';
function GetOutputReportLength(pHandle:UINT):UINT;stdcall;external DLL_NAME name 'GetOutputReportLength';

procedure SetReadNotify(pHandle:UINT;pValue:BOOL);stdcall;external DLL_NAME name 'SetReadNotify';
function IsReadNotifyEnabled(pHandle:UINT):BOOL;stdcall;external DLL_NAME name 'IsReadNotifyEnabled';

function IsAvailable(pVendorID,pProductID:UINT):BOOL;stdcall;external DLL_NAME name 'IsAvailable';

implementation

end.
