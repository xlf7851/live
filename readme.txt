1、加入工程编译的第三方库
    1、静态库
       1、json，jsoncpp源码编译
       2、xml，tinyxml2源码编译
    2、动态库
       1、duilib,界面库
       2、

2、不编译的第三方库
   1、动态库
       1、libcurl,下载源码编译后加入工程

3、解决方案目录
   1、lib目录
      1、所有lib放入此目录，
      2、加入工程编译的lib生成在lib子目录，如lib\\release\\xml.lib
      3、不加入编译的直接在lib目录：如lib\\libcurl.lib
   2、可执行程序生成目录，所有dll和exe工程生成到此目录，
   3、temp中间文件目录
      1、所有中间文件生成在此目录的子目录中，如temp\\release\\*.obj
   4、include目录
      1、所有不需编译的第三方库的包含文件放入此目录中，如inlcude\\curl\\
   5、所有工程放在解决方案根目录

4、config目录，主程序运行需要的备份文件
   1、不加入工程编译好的dll文件，如：config\\libcurl\\，子目录再安装64位和x86备份
   2、skin，duilib的xml文件
   3、config，其他配置文件  
   4、cef文件
      

        
   

