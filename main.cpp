#include "main.hpp"
using namespace std;

constexpr unsigned MAX = (unsigned)-1;

namespace
{
    // 默认全局参数
    int T = 6;
    unsigned dtData = 1;
    unsigned maxData = 7;
    unsigned timebase = 4;
}

/*
 * @function     : main
 * @arguments    : void
 * @return       : int
 * @exception    : runtime_error
 * @introduction : 主函数
 */
int main(void)
{
    while (true)
    {
        cout << "提示: 输入任意负数可以结束程序。" << endl;

        try
        {
            cout << "输入数据个数: " << flush;
            cin >> T;

            if (T <= 0)
                throw int(-1);

            cout << "输入递增步长: " << flush;
            cin >> dtData;

            if (dtData <= 0)
                throw int(-1);

            cout << "输入递增上界: " << flush;
            cin >> maxData;

            if (maxData <= 0)
                throw int(-1);

            cout << "输入限制时间: " << flush;
            cin >> timebase;

            if (timebase <= 0)
                throw int(-1);
        }
        catch (int i)
        {
            cout << "退出程序!" << endl;
            return 0;
        }
        catch (int i)
        {
            cout << "其他错误!" << endl
                 << endl;
            continue;
        }

        unsigned *arr = new unsigned[T];

        cout << "请输入" << T << "个非负数据: " << endl;

        for (unsigned i = 0; i < T; ++i)
        {
            cin >> arr[i];

            if (arr[i] < 0)
            {
                while (true)
                {
                    cerr << "请勿输入负数，请重新输入此项!" << endl;
                    cin >> arr[i];

                    if (arr[i] >= 0)
                        break;
                }
            }
        }

        auto pData = new Data(arr, T);

        // 前面是差值，后面是上界
        pData->setConfig(dtData, maxData);

        pData->exec();
        pData->print();

        /* 设置时间限制 */
        pData->setTime(timebase);
        pData->timeCut();
        pData->print();
    }

    return 0;
}

/*
 * @function     : Data
 * @arguments    : unsigned*, unsigned
 * @return       : none
 * @exception    : none
 * @introduction : 构造函数
 */
Data::Data(unsigned *data, unsigned count) : pos(0), dtData(0), MaxData(0), x13(0), timebase(MAX)
{
    this->database = new unsigned *[count];
    for (unsigned i = 0; i < count; ++i)
    {
        this->database[i] = new unsigned[count - i];
        for (unsigned j = 0; j < count - i; j++)
        {
            this->database[i][j] = 0;
        }
    }

    for (unsigned i = 0; i < count; ++i)
    {
        indexMap.push_back(i);
    }

    unsigned xpos = this->pos;

    for (unsigned i = 0; i < count; ++i)
    {
        this->id2value[i] = *data;
        this->database[xpos][i] = *data++;
    }
}

/*
 * @function     : ~Data
 * @arguments    : void
 * @return       : none
 * @exception    : none
 * @introduction : 析构函数
 */
Data::~Data()
{
    delete[] this->database;

    for (unsigned i = 0; i < T; ++i)
    {
        delete[] this->database[i];
    }

    delete[] this->database;
}

/*
 * @function     : exec
 * @arguments    : void
 * @return       : void
 * @exception    : none
 * @introduction : 执行
 */
void Data::exec(void)
{
    while (this->indexMap.size() != 0)
    {
        this->Increase();
    }
}

/*
 * @function     : print
 * @arguments    : void
 * @return       : void
 * @exception    : none
 * @introduction : 执行
 */
void Data::print(void)
{
    cout << "\n输出队列为: ";

    unsigned counter = 0;
    for (auto i : steps)
    {
        if (counter++ < this->timebase)
        {
            if (i == MAX)
            {
                cout << "* ";
                continue;
            }
            else
            {
                cout << i << " ";
                continue;
            }
        }
        else
        {
            break;
        }
    }

    cout << "\n"
         << endl;
}

/*
 * @function     : timeCut
 * @arguments    : void
 * @return       : void
 * @exception    : none
 * @introduction : 转换为限时模式
 */
void Data::timeCut(void)
{
    auto sz = this->steps.size();

    // 如果能装下直接返回
    if (sz <= this->timebase)
        return;

    // freestep: timebase 内的空闲操作数, freebase: timebase 外的有效操作数
    unsigned freestep = 0, freebase = 0;

    // 开辟新的空间，回收没用的内存
    if (1)
    {
        // 临时计数量，退出时自动回收
        unsigned count = 0, max_count = 0;

        // timebase 内的 max 总数
        for (auto i : steps)
        {
            if (count < timebase)
            {
                if (i == MAX)
                    ++max_count;
            }
            else
                break;

            ++count;
        }

        freestep = max_count;
        // 重置计数器
        max_count = 0;

        // timebase 后的 max 总数
        for (; count < steps.size(); ++count)
        {
            if (steps[count] == MAX)
                ++max_count;
        }

        freebase = steps.size() - timebase - max_count;
    }

    // 用于指向 timebase 后的时间步
    auto freeptr = steps.begin() + timebase;

    // 两者有一个归零就退出
    while (freestep != 0 && freebase != 0)
    {
        // 寻找 timebase 内的第一个MAX
        auto it = find(steps.rbegin(), steps.rend(), MAX);

        if (it == steps.rend())
        {
            cerr << "没有找到 MAX 的值,逻辑出现错误!" << endl
                 << "timeCut 失败!" << endl;
            return;
        }

        // Tips: 直接转换成地址值运算，不然会报错
        while (&(*it) - &steps[0] > this->timebase)
        {
            it = find(it - 1, steps.rend(), MAX);

            if (it == steps.rend())
            {
                cerr << "没有找到 MAX 的值,逻辑出现错误!" << endl
                     << "timeCut 失败!" << endl;
                return;
            }
        }

        // 重定向 freeptr到最近一个非 MAX 的数据
        while (*freeptr == MAX)
            ++freeptr;

        // it 指向 timebase 内最后的一个 MAX
        *it = *freeptr++;

        --freebase;
        --freestep;
    }

    steps.erase(steps.begin() + timebase, steps.end());
}

/*
 * @function     : setConfig
 * @arguments    : unsigned, unsigned
 * @return       : void
 * @exception    : none
 * @introduction : 配置步长、最大值函数
 */
void Data::setConfig(unsigned dtData, unsigned MaxData)
{
    this->dtData = dtData;
    this->MaxData = MaxData;
}

/*
 * @function     : setTime
 * @arguments    : unsigned
 * @return       : void
 * @exception    : none
 * @introduction : 配置计时函数
 */
void Data::setTime(unsigned time)
{
    this->timebase = time;
}

/*
 * @function     : reset
 * @arguments    : unsigned*, unsigned
 * @return       : void
 * @exception    : none
 * @introduction : 重置内容
 */
void Data::reset(unsigned *data, unsigned count)
{
    if (database == nullptr)
    {
        throw runtime_error("不允许空赋值");
    }
    for (unsigned i = 0; i < count; ++i)
    {
        this->database[this->pos][i] = *data++;
    }
}

/*
 * @function     : getLine
 * @arguments    : void
 * @return       : unsigned*
 * @exception    : none
 * @introduction : 获取本行内容
 */
void Data::getLine(unsigned **ptr)
{
    *ptr = this->database[this->pos];
}

/*
 * @function     : getLineByID
 * @arguments    : unsigned
 * @return       : unsigned*
 * @exception    : none
 * @introduction : 获取指定行内容
 */
void Data::getLineByID(unsigned ID, unsigned **ptr)
{
    // 越界操作
    if (ID >= T)
    {
        *ptr = nullptr;
    }

    *ptr = this->database[ID];
}

/*
 * @function     : increasePos
 * @arguments    : unsigned
 * @return       : void
 * @exception    : none
 * @introduction : 进程递增指定步长
 */
void Data::increasePos(unsigned sp)
{
    this->pos += sp;
}

/*
 * @function     : Increase
 * @arguments    : void
 * @return       : void
 * @exception    : runtime_error
 * @introduction : 时间片向前推进
 */
void Data::Increase(void)
{
    unsigned *nowLine = nullptr;
    getLine(&nowLine);
    if (nowLine == nullptr)
    {
        cerr << "当前行数获取失败" << endl;
    }

    if (MaxData == 0 || dtData == 0)
        throw runtime_error("未正确配置参数");

    // flag 是新一行的 index， node 是旧的 index
    vector<unsigned> node, flag;

    for (unsigned i = 0; i < T - this->pos; ++i)
    {
        auto tmp = nowLine[i];
        if (tmp > MaxData - dtData)
        {
            flag.push_back(i);
            node.push_back(indexMap[i]);
        }
    }

    auto sz = node.size();
    // 如果没有快要超过限制的
    if (sz == 0)
    {
        // 表示不做操作
        steps.push_back(MAX);
    }
    else if (sz == 1)
    {
        steps.push_back(node[0]);
    }
    else if (sz > 1)
    {
        bool first = true;

        for (auto i : node)
        {
            if (first)
            {
                steps.push_back(i);
                first = false;
                continue;
            }
            auto lastMax = find(steps.rbegin(), steps.rend(), MAX);

            // 如果找到了上一个MAX
            if (lastMax != steps.rend())
            {
                *lastMax = i;
            }
            // 如果前面没有了就直接舍弃
            else
                continue;
        }
    }

    if (sz > 0)
    {
        // 需要拷贝进去的行
        unsigned *newLine = nullptr;

        getLineByID(this->pos + sz, &newLine);
        if (newLine == nullptr)
        {
            cerr << "下一行数获取失败" << endl;
        }

        set<unsigned> del;
        for (unsigned i = 0, j = 0; i < T - this->pos; ++i)
        {
            // 如果她目前还没有满
            if (find(flag.begin(), flag.end(), i) == flag.end())
            {
                auto tmp = id2value[indexMap[i]] + x13 * dtData;
                newLine[j++] = tmp;
            }
            // 如果已经被删了就continue
            else
            {
                cout << "插入元素: " << indexMap[i] << endl;
                del.insert(indexMap[i]);
            }
        }

        for (unsigned i = 0, j = 0; i < T - this->pos - sz; ++i)
        {
            cout << newLine[i] << "\t";
        }
        cout << endl;

        for (auto rbeg = del.rbegin(); rbeg != del.rend(); rbeg++)
        {
            auto p = find(indexMap.begin(), indexMap.end(), *rbeg);
            auto index = p - indexMap.begin();
            indexMap.erase(indexMap.begin() + index);
        }

        // 更新pos值
        this->pos += sz;
    }

    for (unsigned i = 0; i < T - this->pos; ++i)
    {
        auto a = database[this->pos][i];
        this->database[this->pos][i] += this->dtData;
    }
    x13 += 1;
}

/*
 * @function     : increasePos
 * @arguments    : void
 * @return       : void
 * @exception    : none
 * @introduction : 进程递增默认步长
 */
void Data::increasePos(void)
{
    this->pos += 1;
}