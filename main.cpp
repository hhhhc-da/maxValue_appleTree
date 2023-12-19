#include "main.hpp"
using namespace std;

constexpr unsigned MAX = (unsigned)-1;

/*
 * @function     : main
 * @arguments    : void
 * @return       : int
 * @exception    : runtime_error
 * @introduction : ������
 */
int main(void)
{
    // �������� main ����
    constexpr int N = 6;
    unsigned *arr = new unsigned[N];

    while (true)
    {
        cout << "������6������: (���� -1 �˳�����)" << endl;
        cin >> arr[0];
        if (arr[0] == (unsigned)-1)
        {
            cout << "�����˳�" << endl;
            break;
        }

        for (unsigned i = 1; i < 6; ++i)
        {
            cin >> arr[i];
        }

        auto pData = new Data<N>(arr, 6);

        // ǰ���ǲ�ֵ���������Ͻ�
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
 * @introduction : ���캯��
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
 * @introduction : ��������
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
 * @introduction : ִ��
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
 * @introduction : ִ��
 */
template <unsigned T>
void Data<T>::print(void)
{
    cout << "\n�������Ϊ: ";

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
 * @introduction : ת��Ϊ��ʱģʽ
 */
template <unsigned T>
void Data<T>::timeCut(void)
{
    auto sz = this->steps.size();

    // �����װ��ֱ�ӷ���
    if (!(sz > this->timebase))
        return;

    // ���ں����ǲ����ܵ����ٽ�ֵ�ģ����Կ���ֱ�Ӳ���ǰ��Ŀ�λ(����Ҳ����ν����Ϊ��ֵ��ͬ)
    auto it = find(steps.rbegin(), steps.rend(), MAX);
    // ֱ��ת���ɵ�ֵַ���㣬��Ȼ�ᱨ��
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
        // Ѱ����һ��
        it = find(it - 1, steps.rend(), MAX);
    }
}

/*
 * @function     : setConfig
 * @arguments    : unsigned, unsigned
 * @return       : void
 * @exception    : none
 * @introduction : ���ò��������ֵ����
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
 * @introduction : ���ü�ʱ����
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
 * @introduction : ��������
 */
template <unsigned T>
void Data<T>::reset(unsigned *data, unsigned count)
{
    if (database == nullptr)
    {
        throw runtime_error("������ո�ֵ");
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
 * @introduction : ��ȡ��������
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
 * @introduction : ��ȡָ��������
 */
template <unsigned T>
void Data<T>::getLineByID(unsigned ID, unsigned **ptr)
{
    // Խ�����
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
 * @introduction : ���̵���ָ������
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
 * @introduction : ʱ��Ƭ��ǰ�ƽ�
 */
template <unsigned T>
void Data<T>::Increase(void)
{
    unsigned *nowLine = nullptr;
    getLine(&nowLine);
    if (nowLine == nullptr)
    {
        cerr << "��ǰ������ȡʧ��" << endl;
    }

    if (MaxData == 0 || dtData == 0)
        throw runtime_error("δ��ȷ���ò���");

    // flag ����һ�е� index�� node �Ǿɵ� index
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
    // ���û�п�Ҫ�������Ƶ�
    if (sz == 0)
    {
        // ��ʾ��������
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

            // ����ҵ�����һ��MAX
            if (lastMax != steps.rend())
            {
                *lastMax = i;
            }
            // ���ǰ��û���˾�ֱ������
            else
                continue;
        }
    }

    if (sz > 0)
    {
        // ��Ҫ������ȥ����
        unsigned *newLine = nullptr;

        getLineByID(this->pos + sz, &newLine);
        if (newLine == nullptr)
        {
            cerr << "��һ������ȡʧ��" << endl;
        }

        set<unsigned> del;
        for (unsigned i = 0, j = 0; i < T - this->pos; ++i)
        {
            // �����Ŀǰ��û����
            if (find(flag.begin(), flag.end(), i) == flag.end())
            {
                auto tmp = id2value[indexMap[i]] + x13;
                newLine[j++] = tmp;
            }
            // ����Ѿ���ɾ�˾�continue
            else
            {
                cout << "����Ԫ��: " << indexMap[i] << endl;
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

        // ����posֵ
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
 * @introduction : ���̵���Ĭ�ϲ���
 */
template <unsigned T>
void Data<T>::increasePos(void)
{
    this->pos += 1;
}