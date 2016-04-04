

#编译器
export SCP_COMPILER=gcc

#头文件目录
export SCP_INC_PATH=$SCP_ROOT_PATH/include
#二进制文件输出目录
export SCP_OPT_PATH=$SCP_ROOT_PATH/output
#.c源文件所在目录
export SCP_SRC_PATH=$SCP_ROOT_PATH/source
#库文件所在目录
export SCP_LIB_PATH=$SCP_ROOT_PATH/lib


echo "export COMPILER=$SCP_COMPILER"
echo "export SCP_ROOT_PATH = $SCP_ROOT_PATH"
echo "export SCP_INC_PATH = $SCP_INC_PATH"
echo "export SCP_SRC_PATH = $SCP_SRC_PATH"
echo "export SCP_LIB_PATH = $SCP_LIB_PATH"
echo "export SCP_OPT_PATH = $SCP_OPT_PATH"
