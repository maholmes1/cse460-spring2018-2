//killprocess
if [ -n "$(ps -e -f | grep $1 | grep -v grep | grep -v $0 | awk '{print
$2}')" ]
 then
 //runs as long as process is still running
 for pid in $(ps -e -f | grep $1 | grep -v grep | grep -v $0 | awk '{print
$2}')
 do
 kill $pid;
 echo Process $1 killed!;
 done
else
 echo Process $1 not running!;
fi
