库格式
vs2015 debug 64

使用glm 3d 数学库 进行开发，先不自己写数学库;
版本为 glm-0.9.8.4

使用 stb_image.h用来加载图形文件；

如果要是程序可执行，必须将exe文件拷贝到bin文件夹下，否则找不到路径；

2017年6月17日
添加EffectMgr
处理目录的问题
支持线条
支持纯色
支持文字

使用 freetype-2.8 渲染字体
http://learnopengl-cn.readthedocs.io/zh/latest/06%20In%20Practice/02%20Text%20Rendering/
https://learnopengl.com/#!In-Practice/Text-Rendering

注意这些 游戏库
http://blog.csdn.net/xiaoxiaoyeyaya/article/details/42541419

fmod 声音库
音频引擎FMOD学习记录
http://blog.csdn.net/fengyhack/article/details/43318575

2017年6月22日
CRefCounter可能有问题，保证对内存的检测，当最终退出时引用计数没有成为0则直接报错，需要这个机制来检测内存的使用情况；

