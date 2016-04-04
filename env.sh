#编译器
export SCP_COMPILER=gcc
#工程的根目录
export SCP_ROOT_PATH=$(pwd)
#头文件目录
export SCP_INC_PATH=$SCP_ROOT_PATH/include
#二进制文件输出目录
export SCP_OPT_PATH=$SCP_ROOT_PATH/output
#模块所在目录
export SCP_MODULE_PATH=$SCP_ROOT_PATH/module
#基于模块代码所在目录
export SCP_SRC_PATH=$SCP_ROOT_PATH/src
#库文件所在目录
export SCP_LIB_PATH=$SCP_ROOT_PATH/lib


#根据库要编译的自己的工程目录
export SCP_PROJECT_PATH=$SCP_ROOT_PATH/scptest


echo "export COMPILER=$SCP_COMPILER"
echo "export SCP_ROOT_PATH = $SCP_ROOT_PATH"
echo "export SCP_MODULE_PATH = $SCP_MODULE_PATH"
echo "export SCP_INC_PATH = $SCP_INC_PATH"
echo "export SCP_SRC_PATH = $SCP_SRC_PATH"
echo "export SCP_LIB_PATH = $SCP_LIB_PATH"
echo "export SCP_OPT_PATH = $SCP_OPT_PATH"
