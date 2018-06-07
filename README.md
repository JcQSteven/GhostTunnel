# GhostTunnel
- 首先这个概念是360提出来的，但是一直没有源码所以我尝试自己复现
- 我在Windows上复现过，但是对于response帧的解析一直不太好所以没有公开源码，我认为这个是有攻击性的
- 但是后来我的学弟在github昨天也做出来了windows上的GhostTunnel复现，这是他的git（https://github.com/MRdoulestar/yunsle_ghost_tunnel）
- 我已经被身边的人吐槽了没有第一时间把代码放出来
- 学弟的windows代码写的比我好，在已经得到他允许的情况下，我会对他windows代码进行修改和稳定
- 这个项目会正式开源，技术无罪。一直到稳定使用为止。
- 非常感谢学弟把scapy的部分写完了。我原来测试是一直用hostapd 来测试。

# 主要问题
- windows、mac都还不稳定
- 主要问题是beancon帧和response帧在空气中的残留导致
- 接下来会对windows下的受控端进行优化和改进、能够建立长连接控制
- 大佬轻喷
