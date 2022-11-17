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
  
送るファイル数を入力：　と表示されるので1以上の整数を入力
その後　送るファイル名を入力：　と表示されるのでclientファイル内にある test.jpg を入力
1000バイトずつ順序番号(seq_num)とともにACKが返ってきたかが表示される。
正常に送受信していれば　ACK received
タイムアウトであれば　ACK timeout
その他のエラーであれば　ACK error
が表示される。
  
順序番号は最初のファイル名を送るときは-2
実際に送るファイルでは1から始まり1000バイトごとに1ずつ増える。
複数のファイルを送るとき、送る最後のファイル以外では
そのファイルの最後に送るデータの順序番号は-1になり、
最後のファイルの最後に送るデータの順序番号は0になる。
（1つのファイルのみを送る場合は、-1は出てこない）
  

