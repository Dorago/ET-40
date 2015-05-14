SET PATH=C:\Keil\ARM\ARMCC\bin\;C:\Program Files (x86)\Microchip\mplabc18\v3.46\mpasm;C:\Program Files (x86)\Microchip\mplabc18\v3.46\bin;C:\Program Files\Common Files\Microsoft Shared\Windows Live;C:\Program Files (x86)\Common Files\Microsoft Shared\Windows Live;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program Files (x86)\Intel\Services\IPT\;C:\Program Files (x86)\Windows Live\Shared;C:\Program Files (x86)\Symantec\VIP Access Client\;c:\Program Files (x86)\Common Files\Roxio Shared\OEM\DLLShared\;c:\Program Files (x86)\Common Files\Roxio Shared\OEM\DLLShared\;c:\Program Files (x86)\Common Files\Roxio Shared\OEM\12.0\DLLShared\;c:\Program Files (x86)\Roxio 2010\OEM\AudioCore\;c:\Program Files (x86)\Microsoft SQL Server\90\Tools\binn\;C:\Program Files (x86)\Intel\OpenCL SDK\2.0\bin\x86;C:\Program Files (x86)\Intel\OpenCL SDK\2.0\bin\x64;C:\Program Files (x86)\Microchip\MPLAB C32 Suite\bin;C:\Program Files (x86)\Microchip\MPLAB C30\bin;C:\Program Files (x86)\QuickTime\QTSystem\;C:\w32tex\bin;C:\Program Files (x86)\gs\gs9.16\bin;C:\Program Files (x86)\gs\gs9.16\lib;C:\gs\gs7.07\bin;C:\gs\gs7.07\lib;C:\Program Files (x86)\Microchip\mplabc18\v3.46\mpasm;C:\Program Files (x86)\Microchip\mplabc18\v3.46\bin
SET CPU_TYPE=LPC1114/102
SET CPU_VENDOR=NXP (founded by Philips)
SET UV2_TARGET=mbed NXP LPC1114
SET CPU_CLOCK=0x00B71B00
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\build\main.__i"
"C:\Keil\ARM\ARMCC\bin\ArmLink" --Via ".\build\mbed_project.lnp"
"C:\Keil\ARM\ARMCC\bin\fromelf.exe" ".\build\mbed_project.axf" --i32combined --output ".\build\mbed_project.hex"
fromelf --bin -o mbed_project_LPC1114.bin build\mbed_project.axf
