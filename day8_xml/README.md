- 功能:对RSS文件进行解析,获取内容,转存为pagelib.dat格式的文件,并对文章正文进行正则匹配,去除乱码等

- 解析内容包括:文件所有channel的titile,description,language,lastBuildDate标签,每个item的title,description,content标签.

- 注意:pagelib.dat文件在代码中以追加形式打开,因此运行之前最好先确保目录下没有该文件

- 测试文件:coolshell.xml为测试文件,该测试文件只有一个channel,但是本程序同样可以对多个channel的文件进行解析

- 正则匹配只做了除去<>尖括号内的内容,并去除了"&nsbp"字符串,针对不同的RSS文件需要对replace函数进行一定的修改.

- 实现原理:
    - translate函数打开xml文件,解析里面的所有channel标签和每个channel下的titile,description,language,lastBuildDate标签,写入pagelib.dat文件,然后解析所有item标签,信息存入infomation数组中.
    - traverse函数负责写infomation数组,并在写之前对content内容进行正则匹配修改.
