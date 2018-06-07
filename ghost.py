import os
import time
import subprocess
import random
import string
import psutil
def createPayload(ssid,cmd):
    cmd=cmd.rstrip().lstrip()
    magic_code='ccc'
    head_id='dd'
    length=str(len(ssid+magic_code+cmd))
    raw_payload=length+ssid+magic_code+cmd
    payload=head_id+(''.join([hex(ord(c)).replace('0x','') for c in raw_payload]))
    return payload
    

def createSsid():
    ssid = ''.join(random.sample(string.ascii_letters, 5))
    return ssid
    pass

def setValue(file_name,content,ssid,payload):
    
    ssid_index=content.index('ssid')
    former_ssid=content[ssid_index+5:ssid_index+11]
    content=content.replace(former_ssid,ssid+'\n')
    
    vendor_index=content.index('vendor_elements')
    vendor_next_index=content.index('wpa=1')
    former_payload=content[vendor_index+16:vendor_next_index]
    new_content=content.replace(former_payload,payload+'\n')
    changeFile(file_name,new_content)
    pass

def creatHostapd():
    p=subprocess.Popen('sudo hostapd hostapd.conf',shell=True,stdout=subprocess.PIPE)
    return p.pid
    pass

def killHostapd(pid):
    subprocess.Popen("kill "+str(pid-2),shell=True,stdout=subprocess.PIPE)
    subprocess.Popen("kill -9 "+str(pid),shell=True,stderr=subprocess.PIPE,stdout=subprocess.PIPE)
    pass

def changeFile(file_name,content):
    f=open(file_name,'w')
    f.writelines(content)
    f.close()

if __name__ == "__main__":
    for proc in psutil.process_iter():
        if proc.name()=='hostapd':
            print(proc)
    # file_name='hostapd.conf'
    # flag=False
    # while 1:
    #     cmd=raw_input('#>')
    #     if flag:
    #         killHostapd(pid)
    #         time.sleep(0.1)
    #         creatHostapd()
    #     if cmd='exit':
    #         exit(0)
    #     next_ssid=createSsid()
    #     payload=createPayload(next_ssid,cmd)
    #     f=open(file_name,'r')
    #     content=f.read()
    #     setValue(file_name,content,next_ssid,payload)
    #     pid=creatHostapd()
    #     flag=True