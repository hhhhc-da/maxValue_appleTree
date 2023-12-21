#include "main.hpp"
using namespace std;

constexpr unsigned MAX = (unsigned)-1;

namespace
{
    // Ĭ��ȫ�ֲ���
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
 * @introduction : ������
 */
int main(void)
{
    while (true)
    {
        cout << "��ʾ: �������⸺�����Խ�������" << endl;

        try
        {
            cout << "�������ݸ���: " << flush;
            cin >> T;

            if (T <= 0)
                throw int(-1);

            cout << "�����������: " << flush;
            cin >> dtData;

            if (dtData <= 0)
                throw int(-1);

            cout << "��������Ͻ�: " << flush;
            cin >> maxData;

            if (maxData <= 0)
                throw int(-1);

            cout << "��������ʱ��: " << flush;
            cin >> timebase;

            if (timebase <= 0)
                throw int(-1);
        }
        catch (int i)
        {
            cout << "�˳�����!" << endl;
            return 0;
        }
        catch (int i)
        {
            cout << "��������!" << endl
                 << endl;
            continue;
        }

        unsigned *arr = new unsigned[T];

        cout << "������" << T << "���Ǹ�����: " << endl;

        for (unsigned i = 0; i < T; ++i)
        {
            cin >> arr[i];

            if (arr[i] < 0)
            {
                while (true)
                {
                    cerr << "�������븺�����������������!" << endl;
                    cin >> arr[i];

                    if (arr[i] >= 0)
                        break;
                }
            }
        }

        auto pData = new Data(arr, T);

        // ǰ���ǲ�ֵ���������Ͻ�
        pData->setConfig(dtData, maxData);

        pData->exec();
        pData->print();

        /* ����ʱ������ */
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
 * @introduction : ���캯��
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
 * @introduction : ��������
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
 * @introduction : ִ��
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
 * @introduction : ִ��
 */
void Data::print(void)
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
void Data::timeCut(void)
{
    auto sz = this->steps.size();

    // �����װ��ֱ�ӷ���
    if (sz <= this->timebase)
        return;

    // freestep: timebase �ڵĿ��в�����, freebase: timebase �����Ч������
    unsigned freestep = 0, freebase = 0;

    // �����µĿռ䣬����û�õ��ڴ�
    if (1)
    {
        // ��ʱ���������˳�ʱ�Զ�����
        unsigned count = 0, max_count = 0;

        // timebase �ڵ� max ����
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
        // ���ü�����
        max_count = 0;

        // timebase ��� max ����
        for (; count < steps.size(); ++count)
        {
            if (steps[count] == MAX)
                ++max_count;
        }

        freebase = steps.size() - timebase - max_count;
    }

    // ����ָ�� timebase ���ʱ�䲽
    auto freeptr = steps.begin() + timebase;

    // ������һ��������˳�
    while (freestep != 0 && freebase != 0)
    {
        // Ѱ�� timebase �ڵĵ�һ��MAX
        auto it = find(steps.rbegin(), steps.rend(), MAX);

        if (it == steps.rend())
        {
            cerr << "û���ҵ� MAX ��ֵ,�߼����ִ���!" << endl
                 << "timeCut ʧ��!" << endl;
            return;
        }

        // Tips: ֱ��ת���ɵ�ֵַ���㣬��Ȼ�ᱨ��
        while (&(*it) - &steps[0] > this->timebase)
        {
            it = find(it - 1, steps.rend(), MAX);

            if (it == steps.rend())
            {
                cerr << "û���ҵ� MAX ��ֵ,�߼����ִ���!" << endl
                     << "timeCut ʧ��!" << endl;
                return;
            }
        }

        // �ض��� freeptr�����һ���� MAX ������
        while (*freeptr == MAX)
            ++freeptr;

        // it ָ�� timebase ������һ�� MAX
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
 * @introduction : ���ò��������ֵ����
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
 * @introduction : ���ü�ʱ����
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
 * @introduction : ��������
 */
void Data::reset(unsigned *data, unsigned count)
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
void Data::getLine(unsigned **ptr)
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
void Data::getLineByID(unsigned ID, unsigned **ptr)
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
void Data::increasePos(unsigned sp)
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
void Data::Increase(void)
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
        if (tmp > MaxData - dtData)
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
                auto tmp = id2value[indexMap[i]] + x13 * dtData;
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
void Data::increasePos(void)
{
    this->pos += 1;
}