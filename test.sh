#/bin/bash

set -x

make clean
make

./makeFileSystem 4 mySystem.data # you may change 4 to a reasonable value
./fileSystemOper mySystem.data mkdir "\usr"
./fileSystemOper mySystem.data dir "\usr\ysa"
./fileSystemOper mySystem.data mkdir "\usr\ysa"
./fileSystemOper mySystem.data mkdir "\bin\ysa" # Should print error!
./fileSystemOper mySystem.data write "\usr\ysa\file1" linuxData.data
./fileSystemOper mySystem.data write "\usr\file2" linuxData.data
./fileSystemOper mySystem.data write "\file3" linuxData.data
./fileSystemOper mySystem.data dir "\\" # Should list 1 dir, 1 file
./fileSystemOper mySystem.data dir "\usr"
./fileSystemOper mySystem.data dir "\usr\ysa"
./fileSystemOper mySystem.data del "\usr\ysa\file1"
./fileSystemOper mySystem.data dumpe2fs
./fileSystemOper mySystem.data read "\usr\file2" linuxData_read.data
./fileSystemOper mySystem.data rmdir "\usr\ysa"
./fileSystemOper mySystem.data dir "\usr\ysa"
./fileSystemOper mySystem.data dir "\usr"
cmp linuxData.data linuxData_read.data # Should not print any difference