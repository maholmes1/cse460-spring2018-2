if ps ax | grep -v grep | grep test1 > \dev\null
then
ps -e -f | grep test1 | grep -v grep | awk '{print $2}' | xargs kill
else
echo "That process does not exist."
fi
