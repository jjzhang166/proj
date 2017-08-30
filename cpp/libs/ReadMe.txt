------------------------------------------------------------------------------------------------------------------------------------------------
库的选择：boost > std > Poco > wxWidgets。
boost的兼容性是最好的，每三个月更新一次，更新最为频繁，所以优先选择它；std标准库里现在才有的，boost老早就有了，比如filesystem等，所以还是要先选择boost；
Poco像boost一样，文件的依赖做得比较好；wxWidgets一般要做wxInitializer初始化，即把所有东西都给初始化了，所以最后才是选择它。
------------------------------------------------------------------------------------------------------------------------------------------------
boost Version 1.62.0

//vs2012
bjam install stage --toolset=msvc-11.0 --stagedir="F:\proj\cpp\libs\boost_lib" link=static runtime-link=static threading=multi debug release

//vs2013
bjam install stage --toolset=msvc-12.0 --stagedir="F:\proj\cpp\libs\boost_lib" link=static runtime-link=static threading=multi debug release

//设置zlib目录,--with-iostreams
set  ZLIB_SOURCE="F:\proj\cpp\libs\zlib-1.2.8"

//只编译只定的库
bjam install stage --toolset=msvc-12.0 --stagedir="F:\proj\cpp\libs\boost_lib" --with-serialization link=static runtime-link=static threading=multi debug release

//以上都没有PDB
//vs2013调试版
bjam install stage --toolset=msvc-12.0 --build-type=complete --stagedir="F:\proj\cpp\libs\boost_lib" link=static runtime-link=static threading=multi debug release

如何去编译boost，去看有道笔记。

以上是编译静态链接的，编译静态链接需要：link=shared runtime-link=shared，像下面这样：
bjam install stage --toolset=msvc-12.0 --stagedir="F:\proj\cpp\libs\boost_lib" --with-python link=shared runtime-link=shared threading=multi debug release
bjam install stage --toolset=msvc-12.0 --build-type=complete --stagedir="F:\proj\cpp\libs\boost_lib" link=shared runtime-link=shared threading=multi debug release
------------------------------------------------------------------------------------------------------------------------------------------------
