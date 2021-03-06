---
style: summer
---
# Iptables

## 开启转发功能

多网卡主机，主要开启了ip_forward,就可以实现转发

```
echo 1 > /proc/sys/net/ipv4/ip_forward
```
默认网关,如果目的地不是本机网段，走默认网关
```
route add default gw 192.168.4.1 dev eth0
```

##  链表规则概念
### 链 

    + 概念：规则的合集，数据发送需要走过一个个关卡，链就是关卡的概念。
    + 种类：
        + prerouting
        + input
        + output
        + forward
        + postrouting
![路由](%E8%B7%AF%E7%94%B1.png)

### 表
	+ 概念：相同规则放在同一个表中
	+ 种类：
		+ filter :负责过滤
		+ nat：网络地址转换
		+ mangle : 拆解报文，做修改
		+ raw : 关闭nat表上的连接追踪机制
	+ 表的优先级： raw -> mangle -> nat ->filter

### 规则
	+ 概念：匹配条件和处理动作
	+ 匹配条件：
		+ 基本匹配条件： 目的地和源地址的ip
		+ 扩展匹配条件：端口号，协议
	+ 动作类型
		+ ACCEPT 允许数据包通过
		+ DROP 直接丢弃。
		+ REJECT 拒绝
		+ SNAT 源地址转换
		+ MASQUERADE SNAT的特殊形式，不需要指定源地址是多少
		+ DNAT 目的地转换
		+ REDIRECT 在本机做端口映射
		+ LOG 记入日志

##  基本使用方法

+ 查看某张表中的规则
```
iptables -vt 表 -L --line-num
```
+ 查看某条链中的规则（如果不指定表默认是filter）
```
iptables -vt 表 -L 链 --line-num
```
+ 插入某个规则
```
iptables -t 表 -I 链 规则(-s 源地址) -j 动作
```

+ 追加某个规则
```
iptables -t 表 -A 链 规则(-s 源地址) -j 动作
```
+ 删除某个表中的某条链某个规则
```
iptables -t 表 --line-num -vnL 链 // 查看表中链的规则编号
iptables -t 表 -D 链 编号
```
+ 清空链
```
iptables -t 表 -F 链
```
+ 修改规则（**有坑**）
```
iptables -t 表 -R 链 编号 规则 动作
```
+ 修改链的默认动作
```
iptables -t 表 -P 链 动作
```
+ 保存规则
```
iptables-save > /tmp/iptables
```
+ 导入规则
```
iptables-restore < /tmp/iptables
```
##  匹配规则详解
+ 多ip匹配
```
iptables -t 表 -A 链 -s IP1,IP2 动作
iptables -t 表 -A 链 -s 192.168.0.1/24 动作 // 匹配网段
```
+ 取反
```
iptables -t 表 -A 链 !-s IP 动作
```
**注意**
在filter表中添加一条规则，如果不是192.168.1.136 则ACCEPT：
```
iptables -A INPUT !-s 192.168.1.136 -j ACCEPT
```
**他不代表192.168.1.136就要DROP或REJECT**，192.168.1.136的数据由于没有匹配到规则，他会使用默认动作。INPUT链的默认动作是ACCEPT，所以192.168.1.136也是ACCEPT

+ 基本匹配条件的种类 (一条规则中，不同条件是与的关系）
	+ 源地址匹配 -s
	+ 目的地地址匹配 -d
	+ 协议匹配 -p 
		+ 协议种类 tcp，upd，udplite，icmp，icmpv6，esp，ah，sctp，mh
	+ 网卡匹配
		+ 流入的网卡 -i 可使用的链 PREROUTING,INPUT,FORWARD
		+ 流出的网卡 -o 可使用的链 POSTROUTING, OUTPUT ,FORWARD

+ 扩展匹配条件 
	+ 扩展模块使用 使用 -m
	+ 扩展模块 -m tcp 
		+ 使用模块tcp，必须指定协议 -p ,当协议也是tcp时，-m tcp可以省略
			iptables -t 表 -A 链 -p tcp -m tcp --dport 22 -j DROP
			iptables -t 表 -A 链 -p tcp --dport 22 -j DROP

		+ tcp模块可以使用的匹配条件
			+ --dport 22 目标端口
			+ --sport 22 源端口
			+ --dport :22 0~22 目标端口
			+ --sport 22: 22~65535
			+ --tcp-flags 

	+ 扩展模块 -m mulitport 
		+ 多端口不连续匹配,支持tcp和udp 
			iptables -t 表 -A 链  -p tcp -m mulitport --dport 22,23,25 -j DROP
			
	+ 扩展模块 -m iprange
		+ 一个地址段的匹配
			iptables -t 表 -A 链 -m iprange --src-range 192.168.1.24-192.168.1.56 -j DROP
			iptables -t 表 -A 链 -m iprange --dst-range 192.168.1.24-192.168.1.56 -j DROP
	+ 扩展模块 -m string
		
