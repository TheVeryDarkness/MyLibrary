clear && LANG=C pacman -Qi | sed -n '/Name[ ]*: .*/{s/^[^:]*: //;H};/Installed Size[ ]*: .*/{s/^[^:]*: //;H};${x;s/^\n//;p}' | sed -n '{N;s/\n/ /;p}' | awk '{size = $3=="MiB"? $2*1024*1024 : $3=="KiB"? $2*1024 : $2;printf "%s %d (%s %s)\n", $1, size, $2, $3}' | sort -nk2
