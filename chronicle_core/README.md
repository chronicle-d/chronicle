# chronicle

Example config:

```ini
; Settings
[ChronicleSettings]
SshIdleTimeout=500
SshTotalTimeout=500

; Devices
[PE1-Production]
Vendor=Juniper
DeviceName=VSRX
SshVerbosity=0
User=chronicle-runner
Password=qwaszx33
Host=192.168.1.242
Port=22

[PE2-Production]
Vendor=Cisco
DeviceName=CATALYST_8000V
SshVerbosity=0
User=chronicle-runner
Password=qwaszx33
Host=192.168.1.241
Port=22

[Linux-test]
Vendor=Cisco
DeviceName=LINUX_TEST
SshVerbosity=0
User=root
Password=qwaszx33
Host=192.168.1.243
Port=22
```
