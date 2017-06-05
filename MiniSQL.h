#pragma once

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <direct.h>
#include <string>
#include <sstream>
#include <list>
#include <vector>
using namespace std;
#define NULL 0
#define UNIQUE 1
#define PRIMARY 2
#define CHAR 0
#define FLOAT 1
#define INT 2

//	������������һ�������ڱ��еľ�����Ϣ
struct Attribute
{
	string attr_name;
	int attr_type;	//���Ե��������ͣ��ֱ�ΪCHAR, FLOAT, INT
	int attr_key_type;//���Ե��������ͣ��ֱ�ΪPRIMARY, UNIQUE, NULL(EMPTY)
	int attr_len; 	//��������ŵ����ݵĳ��ȣ��������CHAR���򳤶ȱ��Ϊ1
	int attr_id;    //�����ڱ����ǵڼ���
};

//	��������������Ϣ
struct Table
{
	string table_name;  //����
	int attr_count;				//�������Ե��ܸ���
	Attribute attrs[32];	//�������������б�, ���32������
							//return primary key id
	int getPrimaryKeyId() {
		for (int i = 0; i < attr_count; ++i)
		{
			if (attrs[i].attr_key_type == PRIMARY)
			{
				return i;
			}
		}
		//if no primary key
		return -1;
	}

	int searchAttrId(string att_name) {
		for (int i = 0; i < attr_count; ++i)
		{
			if (attrs[i].attr_name == att_name)
			{
				return i;
			}
		}
		//if no primary key
		return -1;
	}

	int length()
	{
		int len = 0;
		for (int i = 0; i < attr_count; ++i)
		{
			len += attrs[i].attr_len;
		}
		return len;
	}
};

//	���������ж���������Ϣ
struct Condition
{
	string attr_name;	//��������Ӧ��������
	string op_type;		//�������õ��ıȽ�ģʽ���ֱ�Ϊ"<>", "=", ">=", "<=", "<", ">"
	string cmp_value;	//��������Ҫ���бȽϵ�ֵ
};
typedef list<Condition> Condition_list;

//	��������������Ϣ
struct Index
{
	string index_name;
	string table_name;
	string attr_name;	//	��������Ӧ������
};

//insert into card values("", "", 1234.4);
struct Tuple : public Table
{
	string attr_values[32];
};

struct InsertInfo {
	string table_name;
	int value_cnt;
	string values[32];
};