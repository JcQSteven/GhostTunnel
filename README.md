# GhostTunnel
- 首先这个概念是360提出来的，但是一直没有源码所以我尝试自己复现
- 360的PPT地址（https://github.com/360PegasusTeam/PegasusTeam/tree/master/talks）
- 我在Windows上复现过，但是对于response帧的解析一直不太好所以没有公开源码，我认为这个是有攻击性的
- 但是后来我的学弟在github昨天也做出来了windows上的GhostTunnel复现，这是他的git（https://github.com/MRdoulestar/yunsle_ghost_tunnel）
- 我已经被身边的人吐槽了没有第一时间把代码放出来
- 学弟的windows代码写的比我好，在已经得到他允许的情况下，我会对他windows代码进行修改和稳定
- 这个项目会正式开源，技术无罪。一直到稳定使用为止。
- 非常感谢学弟把scapy的部分写完了。我原来测试是一直用hostapd 来测试。
- 我是网络安全通团队的安全研发工程师

# 主要问题
- windows、mac都还不稳定
- 主要问题是beancon帧和response帧在空气中的残留导致
- 接下来会对windows下的受控端进行优化和改进、能够建立长连接控制
- 大佬轻喷

# 文件的说明
- 利用scapy当控制端，我还没有成功过
- 可以利用hostapd中的配置文件进行开启
- PPT里后面是我自己提出的对GhostTunnel长连接的改进
- Windows文件夹中是windows的受控端
- Mac文件夹中是Mac的受控端，swift语言刚接触1周，还很垃圾
- MacDemo这个是我之前做的一些测试，直接无视就好了

# 接下来的工作
- 利用scapy来控制发送帧的数量
- 对Windows受控端的代码进行优化，现在还是学弟的代码，我只是修改了部分函数使得兼容，我原来的代码就不上传了。但是学弟接受response帧中用了WlanGetAvailableNetworkList,利用列表内ssid来查询。这部分可以删除。
- 对于受控端和控制端的交互，我提出了两种想法，一是通过不同SSID来执行不同命令，二是在payload中加入命令ID。以上两种操作都是为了防止空气中的帧残留导致命令重复执行，鬼畜。