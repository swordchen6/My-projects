#pragma once
#include<mysql/mysql.h>
#include<stdio.h>
#include<chrono>
#include<string>
class MysqlConn
{
public:
	MysqlConn();						//��ʼ�����ݿ�����
	~MysqlConn();						//�ͷ����ݿ�����
	bool connect(const char* host,
		const char* user, const char* passwd, const char* db, 
		unsigned int port=3306,const char* unix_socket=nullptr, 
		unsigned long client_flag=0);	//�������ݿ�
	bool update(const char* query);		//�������ݿ� : insert��update�� delete
	bool query(const char* query);		//��ѯ���ݿ�
	bool nextRecond();					//������ѯ�õ��Ľ����
	std::string getValue(int index);	//�õ�������е��ֶ�ֵ
	bool transaction();					//�������
	bool commit();						//�ύ����
	bool rollBack();					//�ع�����
	void refreshAliveTime();			//ˢ����ʼ�Ŀ���ʱ���
	long long getAliveTime();			//�������ʱ��
private:
	void freeResult();
	MYSQL* conn;
	MYSQL_RES* result;
	MYSQL_ROW row;
	std::chrono::steady_clock::time_point aliveTime;
};