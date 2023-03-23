bash -c "sudo mknod /dev/charDevice c 240 0"
bash -c "sudo chmod 666 /dev/charDevice"
echo -p "Character device mounted!"
echo -p "Test it with echo and cat commands to /dev/charDevice"