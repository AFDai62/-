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
送るファイル数を入力：
と表示されるので1以上の整数を入力
送るファイル名を入力：
と表示されるのでclientファイル内にある test.jpg を入力
Sending data of 9 bytes, seq_num=-2....File size is 722757 bytes
1000バイトずつ順序番号とともにACKが返ってきたかが表示される。
正常に受信していれば　ACK received
タイムアウトであれば　ACK timeout
その他のエラーであれば　ACK error
が表示される。
  

