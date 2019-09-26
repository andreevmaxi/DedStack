#include <cstdio>
#include <cstdlib>
#include <string>
#include <cassert>

#ifdef _DEBUG
    #define DEB
#else
    #define DEB if(0)
#endif

typedef int StackElem_t;

int const SizeOfStack = 100;

struct DedStack_t
{
    StackElem_t  data[SizeOfStack]; // data that needs to be saved
    int SizeStack;  // index of next element in data
    DEB int Err;  // number of error !(USED ONLY WHILE DEBUGED)!
    // 0 - all good
    // 1 - is overflow in data array
    // 2 - if in stack maybe a poison element
    // 3 - if size of stack is under the 0
    DEB std::string Name; // name of our stack !(USED ONLY WHILE DEBUGED)!
};

bool StackInit(DedStack_t* aStack);

bool StackPush(DedStack_t* str, StackElem_t tmp);

void ASSERT_OK(DedStack_t* aStack, std::string NowFile, int Line, std::string FuncName);

void DUMP(DedStack_t* aStack, std::string NowFile, int Line, std::string FuncName);

bool IsEmptyStack(DedStack_t* aStack);

StackElem_t PopStack(DedStack_t* aStack);

void StuckDestruck(DedStack_t* aStack);

int main()
    {
    FILE* f = fopen("errors.txt", "w");
    fclose(f);

    DedStack_t dStack1 = {};
    DEB dStack1.Name = "dStack1";
    StackInit(&dStack1);
    StackPush(&dStack1, 10);
    StackPush(&dStack1, -230);
    PopStack(&dStack1);
    StackElem_t x = PopStack(&dStack1);
    printf("x: %d and Is Stack empty: %d", x, IsEmptyStack(&dStack1));
    StuckDestruck(&dStack1);

    return 0;
    }

bool IsEmptyStack(DedStack_t* aStack)
    {
        ASSERT_OK(aStack, __FILE__, __LINE__, __PRETTY_FUNCTION__);
        if(aStack->SizeStack == 0)
            {
            return 1;
            }
        return 0;
    }

StackElem_t PopStack(DedStack_t* aStack)
    {
        ASSERT_OK(aStack, __FILE__, __LINE__, __PRETTY_FUNCTION__);
        if(aStack->SizeStack != 0)
            {
                StackElem_t tmp = aStack->data[ --aStack->SizeStack ];
                aStack->data[ aStack->SizeStack ] = -230;
                return (tmp);
            }
        return (-230);
    }

bool StackInit(DedStack_t* aStack)
    {
        ASSERT_OK(aStack, __FILE__, __LINE__, __PRETTY_FUNCTION__);
        for(int i = 0; i < SizeOfStack; ++i)
            {
                aStack->data[i] = -230; // poison element
            }
        aStack->Err = 0;
        ASSERT_OK(aStack, __FILE__, __LINE__, __PRETTY_FUNCTION__);
        return 0;
    }

bool StackPush(DedStack_t* aStack, StackElem_t tmp)
    {

    ASSERT_OK(aStack, __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if(aStack->SizeStack + 1 > SizeOfStack)
        {
        aStack->Err = 1;
        ASSERT_OK(aStack, __FILE__, __LINE__, __PRETTY_FUNCTION__);
        }
    if(aStack->SizeStack < 0)
        {
        aStack->Err = 3;
        ASSERT_OK(aStack, __FILE__, __LINE__, __PRETTY_FUNCTION__);
        }

    aStack->data[ aStack->SizeStack++ ] = tmp;
    ASSERT_OK(aStack, __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return 1;
    }

void ASSERT_OK(DedStack_t* aStack, std::string NowFile, int Line, std::string FuncName)
    {
    for(int i = 0; i < aStack->SizeStack; ++i)
        {
        if(aStack->data[i] == -230)
            {
            aStack->Err = 2;
            }
        }
    if(aStack->Err != 0)
        {
        DUMP(aStack, NowFile, Line, FuncName);
        }
    return;
    }

void DUMP(DedStack_t* aStack, std::string NowFile, int Line, std::string FuncName)
    {
    FILE* f = fopen("errors.txt", "a");

    std::string status = {};

    if(aStack->Err != 0)
        {
        if(aStack->Err == 2)
            {
            status = "UNK"; // unknown
            } else
            {
            status = "BAD";
            }
        } else
        {
        status = "OK";
        }

    fprintf(f, "StuckDump\n");
    fprintf(f, "Printed from: %s in line: %d in FUCtion: %s\n", NowFile.c_str(), Line, FuncName.c_str());
    fprintf(f, "Stack \"%s\" [%o](%s)\n", aStack->Name.c_str(), (int)aStack, status.c_str());
    fprintf(f, "    {\n");
    fprintf(f, "    size = %d\n", aStack->SizeStack);
    fprintf(f, "    data[%d] = [%o]\n", SizeOfStack, (int)&aStack->data);
    fprintf(f, "        {\n");
    for(int i = 0; i < SizeOfStack; ++i)
        {
        if(i < aStack->SizeStack)
            {
            fprintf(f, "       *");
            }else
            {
            fprintf(f, "        ");
            }
        fprintf(f, "[%d] = %d", i, aStack->data[i]);
        if(aStack->data[i] == -230)
            {
            fprintf(f, "[MB Poison]");
            }
        fprintf(f, "\n");
        }
    fprintf(f, "        }\n");
    fprintf(f, "err = %d (%s)\n", aStack->Err, status.c_str());
    fprintf(f, "    }");

    switch(aStack->Err)
        {
        case 1:
        assert(aStack->Err != 1);
        break;

        case 3:
        assert(aStack->Err != 3);
        break;

        default:
        break;
        }

    fclose(f);
    return;
    }

void StuckDestruck(DedStack_t* aStack)
    {
    ASSERT_OK(aStack, __FILE__, __LINE__, __PRETTY_FUNCTION__);

    while (!IsEmptyStack)
    {
    PopStack(aStack);
    ASSERT_OK(aStack, __FILE__, __LINE__, __PRETTY_FUNCTION__);
    }

    return;
    }
