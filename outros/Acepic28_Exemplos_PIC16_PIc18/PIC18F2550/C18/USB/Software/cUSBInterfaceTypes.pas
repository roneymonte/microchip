unit cUSBInterfaceTypes;

interface

uses
   Windows, Classes, Messages;

const
   WM_HID_EVENT      = WM_APP + 200;
   NOTIFY_PLUGGED    = $0001;
   NOTIFY_UNPLUGGED  = $0002;
   NOTIFY_CHANGED    = $0003;
   NOTIFY_READ       = $0004;

implementation

end.
