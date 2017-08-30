version 3.1.0


nmake -f makefile.vc BUILD=debug SHARED=0 MONOLITHIC=1 UNICODE=1

nmake -f makefile.vc BUILD=release SHARED=0 MONOLITHIC=1 UNICODE=1

或直接在build/msw/目录下打开*.slh文件，用VC进行编译。