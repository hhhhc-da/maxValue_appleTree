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
 * 接口类
 */
template <unsigned T>
class Data
{
public:
    Data() : database(nullptr), pos(0), dtData(0), MaxData(0), x13(0)
    {
        for (unsigned i = 0; i < T; ++i)
        {
            this->database[i] = new unsigned[T - i];
            for (unsigned j = 0; j < T - i; j++)
            {
                this->database[i][j] = 0;
            }
        }

        for (unsigned i = 0; i < T; ++i)
        {
            // 初始化所有东西的下标
            indexMap.push_back(i);
            // 初始化所有绑定值
            id2value[i] = 0;
        }
    }

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

    // 基础数据组（上三角矩阵存储）
    unsigned **database;
    // position数据组，表示目前剩下的几个元素的下标（与 id2value 相对应）
    std::vector<unsigned> indexMap;
    // 数据映射组，建立输入到 value 的查询字典
    std::map<unsigned, unsigned> id2value;
    // 当前行位置，表示已经执行了多少次摘取
    unsigned pos;
    // 递增差值（每次的更新量）、大小上限（不允许超过的极限值）
    unsigned dtData, MaxData;
    // 步骤组（需要执行的步骤）
    std::vector<unsigned> steps;
    // 程序执行计数器（用于动态赋值）
    unsigned x13;
    // 计时器
    unsigned timebase;
};

#endif