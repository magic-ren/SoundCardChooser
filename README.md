# SoundCardChooser
一个android选择声卡、处理音频的库。底层使用了tinyalsa。也是入门学习jni、ndk开发的好项目。

简介：  
java层设计了一个音频播放器接口，通过jni调用c层，在c层实现了指定声卡录制音频、处理音频数据、保存音频文件的功能。播放功能是在java层做的。

作用：   
1）学习tinyalsa库；   
2）学习jni、ndk的开发。（代码做了详细的注释）  

注意：   
本代码需要在特定设备上才能跑起来，所以仅供参考。
