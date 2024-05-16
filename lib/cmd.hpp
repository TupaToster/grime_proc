DEF_CMD (hlt, 0, 0, {exit (0);})

DEF_CMD (push, 1, 1, {

    int arg = 0;
    getArg (cmd, &arg);

    stkPush (arg);
    ERRORCHECK
})

DEF_CMD (pop, 2, 1, {

    int arg = 0;
    getArg (cmd, &arg);

    arg = stkPop ();
    ERRORCHECK
})

DEF_CMD (in, 3, 0, {

    int arg = 0;
    cin >> arg;

    stkPush(arg);
    ERRORCHECK
})

DEF_CMD (out, 4, 0, {

    int arg = 0;

    arg = stkPop ();
    ERRORCHECK

    std::cout << arg << endl;
})

DEF_CMD (add, 5, 0, {

    stkPush (stkPop () + stkPop ());
    ERRORCHECK
})

DEF_CMD (sub, 6, 0, {

    stkPush (stkPop () - stkPop ());
    ERRORCHECK
})

DEF_CMD (mul, 7, 0, {

    stkPush (stkPop () * stkPop ());
    ERRORCHECK
})

DEF_CMD (div, 8, 0, {

    stkPush (stkPop () / stkPop ());
    ERRORCHECK
})

DEF_CMD (jmp, 10, 1, {

    int arg = 0;
    getArg (cmd, &arg);

    if (arg >= code_size) {

        errors[static_cast<int> (Errors::WRONG_JMP_IP)] += 1;
        return;
    }
    ERRORCHECK
})

/// @warning В ip++ может быть ошибка, надо чекнуть епта
#define DEF_JMP(name, num, op)                                      \
DEF_CMD (name, num, 1, {                                            \
                                                                    \
    int arg1 = 0, arg2 = 0;                                         \
    arg1 = stkPop ();                                               \
    arg2 = stkPop ();                                               \
                                                                    \
    ERRORCHECK                                                      \
    if (!(arg2 op arg1)){                                           \
                                                                    \
        ip++;                                                       \
        break;                                                      \
    }                                                               \
                                                                    \
    int arg = 0;                                                    \
    getArg (cmd, &arg);                                             \
                                                                    \
    ERRORCHECK                                                      \
    if (arg >= code_size) {                                         \
                                                                    \
        errors[static_cast<int> (Errors::WRONG_JMP_IP)] += 1;       \
        return;                                                     \
    }                                                               \
                                                                    \
    ip = arg;                                                       \
})

DEF_JMP (ja, 11,  >)

DEF_JMP (jae, 12, >=)

DEF_JMP (jb , 13,  <)

DEF_JMP (jbe, 14, <=)

DEF_JMP (je, 15, ==)

DEF_JMP (jne, 16, !=)

DEF_CMD (call, 17, 1, {

    int arg = 0;
    getArg (cmd, &arg);

    ERRORCHECK

    if (arg > code_size) {

        errors[static_cast<int> (Errors::WRONG_JMP_IP)] += 1;
        return;
    }

    stkCallPush (arg);
    ERRORCHECK

    ip = arg;
})

DEF_CMD (ret, 18, 9, {

    ip = stkCallPop ();

    ERRORCHECK
})

#undef DEF_JMP