server: main.c ./threadpool/threadpool.h ./http/http_conn.cpp ./http/http_conn.h ./lock/locker.h ./log/log.cpp ./log/log.h ./log/block_queue.h ./mysql/sql_connection_pool.cpp ./mysql/sql_connection_pool.h
	g++ -o server main.c ./threadpool/threadpool.h ./http/http_conn.cpp ./http/http_conn.h ./lock/locker.h ./log/log.cpp ./log/log.h ./mysql/sql_connection_pool.cpp ./mysql/sql_connection_pool.h -lpthread -lmysqlclient


clean:
	rm  -r server
