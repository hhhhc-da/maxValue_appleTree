#ifndef NANOKA_MAIN_H
#define NANOKA_MAIN_H

#include <iostream>
#include <memory>
#include <exception>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <memory>
#include <algorithm>

/*
 * �ӿ���
 */
class Data
{
public:
    Data(unsigned *data, unsigned count);
    ~Data();

    void reset(unsigned *data, unsigned count);
    void getLine(unsigned **ptr);
    void getLineByID(unsigned ID, unsigned **ptr);
    void setConfig(unsigned dtData, unsigned MaxData);
    void setTime(unsigned time);
    void Increase(void);
    void exec(void);
    void print(void);
    void timeCut(void);

private:
    void increasePos(unsigned sp);
    void increasePos(void);

    // ���������飨�����Ǿ���洢��
    unsigned **database;
    // position�����飬��ʾĿǰʣ�µļ���Ԫ�ص��±꣨�� id2value ���Ӧ��
    std::vector<unsigned> indexMap;
    // ����ӳ���飬�������뵽 value �Ĳ�ѯ�ֵ�
    std::map<unsigned, unsigned> id2value;
    // ��ǰ��λ�ã���ʾ�Ѿ�ִ���˶��ٴ�ժȡ
    unsigned pos;
    // ������ֵ��ÿ�εĸ�����������С���ޣ����������ļ���ֵ��
    unsigned dtData, MaxData;
    // �����飨��Ҫִ�еĲ��裩
    std::vector<unsigned> steps;
    // ����ִ�м����������ڶ�̬��ֵ��
    unsigned x13;
    // ��ʱ��
    unsigned timebase;
};

#endif