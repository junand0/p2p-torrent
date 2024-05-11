# p2p-torrent

- Torrent-esque P2P file-sharing system.
- Implement torrents using P2P communication.

1. compile

```shell
# LINUX
gcc -o main main.c torrent_functions.o network_functions.o

# MAC
gcc -o main main.c torrent_functions_MAC.o network_functions_MAC.o
```

2. Open two terminals

3. Run “./main 127.0.0.1 1” on one terminal and “./main 127.0.0.1 2” on the other.

4. The downloaded files will be saved under the “Downloaded” directory.
