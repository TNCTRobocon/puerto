#　新プロトコル 

# 最小構成
## 接続
host->slave(i):open 1
slave(i)->host:ok //タイムアウトする場合がある。
## 切断
host->slave(i):close
slave(i)->host:ok
## 確認
host->slave(i):func

