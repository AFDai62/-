# files_send-and-receive

server（受信）側
gcc –o sample_server sample_server_report2.c　でコンパイル
./sample_server　で実行
するとPORT NUMBERが表示される。

client（送信）側
gcc –o sample_client sample_client＿report2.c　でコンパイル
./sample_client <IPaddress> <PORT NUMBER> で実行 （<>は入力しない）
1つのPCで自分自身と通信する場合、IPadressは127.0.0.1
PORT NUMBERは5001のときは入力しなくてもよい
