
ls -la $1 | tr -s " " | cut -d' ' -f 5,9 | sort -nr | head -n 7
