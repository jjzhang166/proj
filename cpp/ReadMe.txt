apps：包括辅助帐号验证服务器(CheckServer)，辅助帐号管理器(CSViewer)，资源文件系统浏览器(AssetExplore)，特征码提取器(ExtractFeatureCode)。
tools：包括注入dll(HijackDll)，通用辅助登陆验证(LoginHlp),录制地图坐标工具(RecordMap)等。
libs：框架和具体项目依赖的外部开源库，包括boost、lua、luabind、luacom、pluto、wxLua、Eigen、openssl、poco、wxWidgets、zlib等。
subs：具体的辅助项目均放在此目录下，包括fifa online3、疾风之刃、怪物猎人online、流放之路等。
Frame：框架所在，包括资源文件系统(AssetSys)，小的外部开源库集合(ExternLib)，非常苛刻编程环境下的框架代码集合(PublicX)，
一般苛刻编程环境下的框架代码集合(PubMix)，普通编程环境下的框架代码集合(PubMix)。
其中苛刻编程环境意思是只能引用少部分系统功能(dll)，不能在该库中加入更多的外部引用动态库。
比如非常苛刻编程环境，要求dll在初始化时只能引用这些系统dll：kernel32.dll、user32.dll等。

以上5个目录可以理解为工程项目的5种分类，其中apps、tools、subs依赖于Frame，而Frame又依赖于libs。

libsetup.props和setup.props是vs的项目属性页配置，libs中的工程大多数都继承自libsetup.props通用属性配置，
apps、tools、subs、Frame中的工程则继承自setup.props通用属性配置。
