# GhostTunnel
- 首先这个概念是360提出来的，但是一直没有源码所以我尝试自己复现
- 360的PPT地址（ https://github.com/360PegasusTeam/PegasusTeam/tree/master/talks ）
- 这个项目会正式开源，技术无罪。一直到稳定使用为止。
- 感谢学弟（ https://github.com/MRdoulestar/yunsle_ghost_tunnel ）对我工作前期的启发。
- 非常感谢我的伙伴jerryma0912( https://github.com/jerryma0912 ) 对我开发工作的支持与帮助。不擅长C++的我很大程度受益于他的帮忙。
- 目前windows版本已经进入稳定演示版本。我们测试的平台是windows 10，测试过部分网卡。但是还未记录文档和测试结果。大部分的DOS指令可以成功执行。
- 我是网络安全通团队的安全研发工程师

# 目前进展
- 实现了无网络连接的情况下远程控制
- 我们已经实现了windows端的稳定控制
- 配合bashbunny可以实现无文件执行exe
- DOS指令执行和powershell指令执行
- python脚本用法`python scapy_ghost.py -c "你的指令“`

# 接下来的工作
- 我们会在其中加入文件传输功能
- 我们需要编写mac端的客户端，暂时没有人可以胜任
- 一份使用的手册
- 请期待我们的项目。当然如果你有更好的想法也欢迎在issue中提出。


