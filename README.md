# files_send-and-receive

server（受信）側<br>
gcc –o sample_server sample_server_report2.c　でコンパイル<br>
./sample_server　で実行するとPORT NUMBERが表示される。


client（送信）側<br>
gcc –o sample_client sample_client＿report2.c　でコンパイル<br>
./sample_client <IPaddress> <PORT NUMBER> で実行 （<>は入力しない）<br>
1つのPCで自分自身と通信する場合、IPadressは127.0.0.1<br>
PORT NUMBERは5001のときは入力しなくてもよい<br>
  
送るファイル数を入力：　と表示されるので1以上の整数を入力<br>
その後　送るファイル名を入力：　と表示されるのでclientファイル内にある test.jpg を入力<br>
1000バイトずつ順序番号(seq_num)とともにACKが返ってきたかが表示される。<br>
正常に送受信していれば　ACK received<br>
タイムアウトであれば　ACK timeout<br>
その他のエラーであれば　ACK error<br>
が表示される。<br>
  
順序番号は最初のファイル名を送るときは-2<br>
実際に送るファイルでは1から始まり1000バイトごとに1ずつ増える。<br>
複数のファイルを送るとき、送る最後のファイル以外では<br>
そのファイルの最後に送るデータの順序番号は-1になり、<br>
最後のファイルの最後に送るデータの順序番号は0になる。<br>
（1つのファイルのみを送る場合は、-1は出てこない）
  

