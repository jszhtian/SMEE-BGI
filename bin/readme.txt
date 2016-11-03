BGIScriptDump.exe
dump commands from BGI Script
用法：
Usage:BGIScriptDump.exe BGIScriptFile
BGIScriptDump.exe后空格加上需要解包的BGI脚本（SMEE这个游戏的脚本均无扩展名！）
等待CMD窗口消失
输出与脚本文件名相同的txt文件。
！！！注意！！！
文件编码为SwiftJIS！！！
同时请检查文本最后一行是否为
Find Target! Text Address:0x????? Allocation Size:28 Text:指定されたラベルは見つかりませんでした。（????问号值可以变更）

如果不是请把封包名告诉我，我回去查程序！

BGIScriptReplace.exe
replace texts to BGI Script
用法
BGIScriptReplace.exe BGI脚本 TXT文件
仅可以改动text:之后文本！！
