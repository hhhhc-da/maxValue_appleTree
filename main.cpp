#include "main.hpp"
using namespace std;

constexpr unsigned MAX = (unsigned)-1;

/*
 * @function     : main
 * @arguments    : void
 * @return       : int
 * @exception    : runtime_error
 * @introduction : 主函数
 */
int main(void)
{
    // 作用域在 main 块内
    constexpr int N = 6;
    unsigned *arr = new unsigned[N];

    while (true)
    {
        cout << "请输入6个数据: (输入 -1 退出程序)" << endl;
        cin >> arr[0];
        if (arr[0] == (unsigned)-1)
        {
            cout << "程序退出" << endl;
            break;
        }

        for (unsigned i = 1; i < 6; ++i)
        {
            cin >> arr[i];
        }

        auto pData = new Data<N>(arr, 6);

        // 前面是差值，后面是上界
        pData->setConfig(1, 7);

        pData->exec();
        pData->print();

        pData->setTime(4);
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
template <unsigned T>
Data<T>::Data(unsigned *data, unsigned count) : pos(0), dtData(0), MaxData(0), x13(0), timebase(MAX)
{
    this->database = new unsigned *[T];
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
template <unsigned T>
Data<T>::~Data()
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
template <unsigned T>
void Data<T>::exec(void)
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
template <unsigned T>
void Data<T>::print(void)
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
template <unsigned T>
void Data<T>::timeCut(void)
{
    auto sz = this->steps.size();

    // 如果能装下直接返回
    if (!(sz > this->timebase))
        return;

    // 由于后面是不可能到达临界值的，所以可以直接插入前面的空位(交换也无所谓，因为差值相同)
    auto it = find(steps.rbegin(), steps.rend(), MAX);
    // 直接转换成地址值运算，不然会报错
    while (&(*it) - &steps[0] > this->timebase)
    {
        it = find(it - 1, steps.rend(), MAX);
    }

    unsigned counter = 0;

    while (it != steps.rend())
    {
        if (this->timebase + counter < steps.size())
        {
            while (*(steps.begin() + this->timebase + counter) == MAX)
            {
                if (this->timebase + counter < steps.size())
                    counter++;
                else
                    return;
            }

            *it = *(steps.begin() + this->timebase + counter++);
        }
        else
            return;
        // 寻找下一个
        it = find(it - 1, steps.rend(), MAX);
    }
}

/*
 * @function     : setConfig
 * @arguments    : unsigned, unsigned
 * @return       : void
 * @exception    : none
 * @introduction : 配置步长、最大值函数
 */
template <unsigned T>
void Data<T>::setConfig(unsigned dtData, unsigned MaxData)
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
template <unsigned T>
void Data<T>::setTime(unsigned time)
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
template <unsigned T>
void Data<T>::reset(unsigned *data, unsigned count)
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
template <unsigned T>
void Data<T>::getLine(unsigned **ptr)
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
template <unsigned T>
void Data<T>::getLineByID(unsigned ID, unsigned **ptr)
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
template <unsigned T>
void Data<T>::increasePos(unsigned sp)
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
template <unsigned T>
void Data<T>::Increase(void)
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
        if (tmp >= MaxData)
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
                auto tmp = id2value[indexMap[i]] + x13;
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
template <unsigned T>
void Data<T>::increasePos(void)
{
    this->pos += 1;
}