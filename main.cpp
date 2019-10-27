#include<stdio.h>
#include<string.h>

const int N = 20;

// структура, описывающая результат выполениия состояния, а именно:
//
struct ReturnType
{
    struct ReturnType(*ResultState)(int); //  это - указатель на функцию. Указывает на следующее состояние, что следует выполнить машине.
    int СellVal; // указывает на текущее значение измененного элемента ленты
    int MachineStep; // Шаг головки: вправо, влево, остались на месте. +1 -> Right, -1 -> left, 0 - stay
    char StateName[40]; // название метода для удобства вывода на экран
};

// структура, описывающая саму нашу машину
struct Machine
{
    struct ReturnType(*MachineState)(int); // это указатель на функцию. Описывает текущее состояние машины.
    int HeaderPos; // координата головки
    char MachineStateName[40]; // название состояния для удобства вывода на экран
};

//--------------------------------------------------
//--------------------PRINTING----------------------
//--------------------------------------------------
// вывод ленты
void PrintCells(int cells[], int n)
{
    for (int i = 0; i < n; i++)
        if (cells[i]  < 0)
            printf("#");
        else printf("%d", *(cells + i));
    printf("\n");
}

// хитрый способ вывода позиции головки в ленте.
void PrintHeaderPos(struct Machine m)
{
    for (int i = 0; i < m.HeaderPos; i++)
        printf("%c", ' ');
    printf("^\n");
}
// объединенная функция печати
void PrintAll(int cells[], Machine M)
{
    printf("State> %s \n", M.MachineStateName);
    PrintCells(cells, N);
    PrintHeaderPos(M);
}
//--------------------------------------------------
//--------------CELLS PREPARE -----------------------
//--------------------------------------------------

// в этой функции заполняете ленту так, как вам нужно для задачи.
// заведите несколько функций если решите в одной программе слепить разные задачи воедино
void PrepareCellsForWork(int cells[])
{
    for (int i = N - 2; i >= N - 5; i--)
        cells[i] = 1;
}

// массив заполняем нулями для гарантии. можно совместить с PrepareCells
void InitCells(int cells[])
{
    for (int i = 0; i < N; i++)
        cells[i] = 0;
}


//--------------------------------------------------
// финальнео состояние, после которого точно завершим работу.
struct ReturnType Q_END(int val)
{
    ReturnType temp = { NULL, 0, 0, "Q_END" };
    return temp;
}

// состоняние "зануления"
struct ReturnType Q_Nullate(int val)
{
    ReturnType RES;
    // если в ячейке 0, то завершаем работу
    if (val == 0)
    {
        RES.ResultState = Q_END;
        RES.СellVal = 0;
        RES.MachineStep = 0;
        strcpy(RES.StateName, "Q_NULLATE");
    }
    // если в ячейке 1, то заменим на 0 и сдвинемся влево. При этом состояние машины останется прежним
    if (val == 1)
    {
        RES.ResultState = Q_Nullate;
        RES.СellVal = 0;
        RES.MachineStep = -1;
        strcpy(RES.StateName, "Q_NULLATE");
    }

    return RES;
}

// стартовое состояние. Именно из него машина начинает работу
struct ReturnType Q_Start(int val)
{
    // в стартовом состоянии мы НЕ меняем значение текущей ячейки и позицию, только Состояние
    ReturnType RES = { Q_Nullate, val, 0, "Q_Start" };
    return RES;
}


// ГЛАВНАЯ ФУНКЦИЯ!!! ВСЯ РАБОТА СОВЕРШАЕТСЯ ЗДЕСЬ!!!
// написана универсально, не думаю что её вам придется менять.
// если придумаете как улучшить - отпишитесь мне пожалуйста и расскажите как вы это сделали.
// общую идею - читайте в описании к этому файле ниже на страничке
// если на пальцах: берем ленту и машину. Вызываем метод реализующий логику работы текущего состояние.
//Получаем результат - применяем его к машине:
// 1 - меняем значение текущей ячейки в ленте
// 2 - сдвигаем головку
// 3 -переписываем состояние машины и его(состояния) название

void RunMachine(int cells[], Machine M)
{
    ReturnType MidResult; // структура для хранения типа
    int step_num = 0; // порядковый номер шага для удобства вывода на экран

    // пока не дойдем до конечного состояния работаем
    while (M.MachineState != Q_END)
    {
        // вызываем метод состояния на выполнение. Передаем ему значение текущей ячейки ленты
        MidResult = M.MachineState(cells[M.HeaderPos]);

        // применяем результаты к нашей машине
        cells[M.HeaderPos] = MidResult.СellVal;
        M.HeaderPos += MidResult.MachineStep;
        M.MachineState = MidResult.ResultState;
        strcpy(M.MachineStateName, MidResult.StateName);

        //условие для выхода чтобы не печатать на экран лишний раз (возможно костыль)
        if (M.MachineState == Q_END) break;

        // лента конечна - проверяем на границы массива.
        // если ваша программа точно работает верно - можете удалить
        // написал чисто для вашего удоства пока будете отлаживать код
        if (M.HeaderPos < 0 || M.HeaderPos >= N)
        {
            printf("ERROR!!!! MachinePos is out of range!!! \n");
            break;
        }

        // выводим на экран текущее состояние программы
        printf("Step # %d \n", step_num);
        step_num++;
        PrintAll(cells, M);
    }
}
//------------------------------------------------------
//-------------------------------------------------------
// УЧЕБНЫЙ ПРИМЕР: напишем машину, которая зануляет все 1

int main()
{
    int cells[N]; // наша лента
    struct Machine M; // наша машина

    // подготовим ленту
    InitCells(cells); // занулим
    PrepareCellsForWork(cells); // заполним значениями под задачу зануления. получим вот такое: 0000011110

    // Установим головку машины на нужное место
    M.HeaderPos = N - 2; // кон
    // переведем машину в стартовое состояние
    M.MachineState = Q_Start;

    //---------
    // запустим
    RunMachine(cells, M);

    return 0;
}