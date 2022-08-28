#include <stdio.h>
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

struct Char2D {
    char** arr;
    int size;
};

int len = 0;

bool find_(struct Char2D temp, char* word)
{
    for (int i = 0; i < temp.size; i++)
    {
        if (strcmp(temp.arr[i], word) == 0)
            return true;
    }
    return false;
}

int partition(char** a, int start, int end)
{
    char* temp = (char*)malloc((len + 1) * sizeof(char));
    char* pivot = a[end];
    int i = (start - 1);
    for (int j = start; j <= end - 1; j++)
    {
        if (!(strcmp(a[j], pivot) > 0))
        {
            i++;
            strcpy(temp, a[i]);
            strcpy(a[i], a[j]);
            strcpy(a[j], temp);
        }
    }
    strcpy(temp, a[i + 1]);
    strcpy(a[i + 1], a[end]);
    strcpy(a[end], temp);
    free(temp);
    return (i + 1);
}

void quick_sort(char** a, int start, int end)
{
    if (start < end)
    {
        int pivot_index = partition(a, start, end);
        quick_sort(a, start, pivot_index - 1);
        quick_sort(a, pivot_index + 1, end);
    }
}

void lexicographical_order(struct Char2D list)
{
    quick_sort(list.arr, 0, list.size - 1);
    for (int i = 0; i < list.size; i++)
    {
        printf("%s", list.arr[i]);
    }
}

void generate_code(char* ref, char* word, char* out)
{
    int refm[256], wordm[256];
    int i = 0;

    for (i = 0; i < len; i++)
    {
        wordm[(int)word[i]] = 0;
        refm[(int)ref[i]] = 0;
        refm[(int)word[i]] = 0;
        wordm[(int)ref[i]] = 0;
    }

    for (i = 0; i < len; i++)
    {
        wordm[(int)word[i]]++;
        refm[(int)ref[i]]++;
    }

    i = 0;
    bool check = false;

    for (int x = 0; x < len; x++)
    {
        int ch = (int)word[x];

        if (wordm[ch] == 1)					// word[i] occurs only once
        {
            if (refm[ch] <= 0)
                out[i] = '/';
            else if (ref[i] == word[i])
                out[i] = '+';
            else
                out[i] = '|';
        }
        else                                        // word[i] occurs again
        {
            if (refm[ch] == 0)
                out[i] = '/';
            else if (ref[i] == word[i])
            {
                refm[ch]--;
                wordm[ch]--;
                out[i] = '+';
            }
            else
            {
                out[i] = '-';
                check = true;
            }
        }
        i++;
    }
    out[i] = 0;

    for (i = 0; i < len && check; i++)
    {
        int index = (int)word[i];
        if (out[i] == '-' && wordm[index] > 0 && refm[index] > 0)
        {
            out[i] = '|';
            wordm[index]--;
            refm[index]--;
        }
        else if (out[i] == '-')
            out[i] = '/';
    }
}

bool check_occurences(char* s1, char* s2, char ch)
{
    int c1 = 0, c2 = 0;
    for (int i = 0; i < len; i++)
    {
        if (s1[i] == ch)
            c1++;
        if (s2[i] == ch)
            c2++;
    }
    if (c2 < c1)
        return true;
    return false;
}

int get_filter(struct Char2D* filtered, char* pattern, char* cmp, char* ref)
{
    for (int i = 0; i < len; i++)
    {
        if (pattern[i] == '/')
        {
            for (int j = 0; j < filtered->size; j++)
            {
                if (filtered->arr[j][i] == cmp[i])
                    strcpy(filtered->arr[j--], filtered->arr[--filtered->size]);
                else if (strchr(filtered->arr[j], cmp[i]) != NULL)
                {
                    if (strchr(ref, cmp[i]) == NULL)
                        strcpy(filtered->arr[j--], filtered->arr[--filtered->size]);
                    else if (check_occurences(filtered->arr[j], ref, cmp[i]))
                        strcpy(filtered->arr[j--], filtered->arr[--filtered->size]);
                }
            }
        }
        else if (pattern[i] == '+')
        {
            for (int j = 0; j < filtered->size; j++)
            {
                if (filtered->arr[j][i] != cmp[i])
                    strcpy(filtered->arr[j--], filtered->arr[--filtered->size]);
            }
        }
        else
        {
            for (int j = 0; j < filtered->size; j++)
            {
                if (strchr(filtered->arr[j], cmp[i]) != NULL)
                {
                    if (filtered->arr[j][i] == cmp[i])
                        strcpy(filtered->arr[j--], filtered->arr[--filtered->size]);
                    else
                    {
                        int count = 0, count2 = 0;
                        for (int x = 0; x < len; x++)
                        {
                            if (filtered->arr[j][x] == cmp[i])
                                count++;
                            if (cmp[x] == cmp[i] && (pattern[x] == '|' || pattern[x] == '+'))
                                count2++;
                        }

                        if (count < count2)
                            strcpy(filtered->arr[j--], filtered->arr[--filtered->size]);
                    }
                }
                else
                    strcpy(filtered->arr[j--], filtered->arr[--filtered->size]);
            }
        }
    }
    return filtered->size;
}

bool check_constraint(char* word, char* ref, struct Char2D cnst, struct Char2D ptrn)
{
    for (int j = 0; j < cnst.size; j++)
    {
        for (int i = 0; i < len; i++)
        {
            if (ptrn.arr[j][i] == '/')
            {
                if (word[i] == cnst.arr[j][i])
                    return false;
                if (strchr(word, cnst.arr[j][i]) != NULL)
                {
                    if (strchr(ref, cnst.arr[j][i]) == NULL)
                        return false;
                    else if (check_occurences(word, ref, cnst.arr[j][i]))
                        return false;
                }
            }
            else if (ptrn.arr[j][i] == '+')
            {
                if (word[i] != cnst.arr[j][i])
                    return false;
            }
            else
            {
                if (strchr(word, cnst.arr[j][i]) != NULL)
                {
                    if (word[i] == cnst.arr[j][i])
                        return false;
                    else
                    {
                        int count = 0, count2 = 0;
                        for (int x = 0; x < len; x++)
                        {
                            if (word[x] == cnst.arr[j][i])
                                count++;
                            if (cnst.arr[j][x] == cnst.arr[j][i] && (ptrn.arr[j][x] == '|' || ptrn.arr[j][x] == '+'))
                                count2++;
                        }

                        if (count < count2)
                            return false;
                    }
                }
                else
                    return false;
            }
        }
    }
    return true;
}

int main(int argc, char* argv[])
{
    char str[100], ref[100], ch, * ptr;
    int max_size = 0, i, temp = 1, count = 0, cnst_size = 0;
    bool check = true;

    for (; fgets(str, 100, stdin) != NULL;)
    {
        str[14] = 0;
        if (check)
            max_size++;
        if (strcmp(str, "+nuova_partita") == 0)
        {
            ptr = fgets(str, 100, stdin);
            ptr = fgets(str, 100, stdin);
            int num = atoi(str);
            if (num > cnst_size)
                cnst_size = num;
            check = false;
        }
        else if (strcmp(str, "+inserisci_fin") == 0)
            check = false;
        else if (strcmp(str, "+inserisci_ini") == 0)
            check = true;
    }
    rewind(stdin);

    ptr = fgets(str, 100, stdin);
    len = atoi(str);
    if (temp == 0)
        printf("Length Not Entered");

    struct Char2D list = (struct Char2D){ .size = 0 }, filtered = (struct Char2D){ .size = 0 }, cnst = (struct Char2D){ .size = 0 }, patterns = (struct Char2D){ .size = 0 };

    list.arr = (char**)malloc(max_size * sizeof(char*));
    filtered.arr = (char**)malloc(max_size * sizeof(char*));
    for (int i = 0; i < max_size; i++)
    {
        list.arr[i] = (char*)malloc((len + 1) * sizeof(char));
        filtered.arr[i] = (char*)malloc((len + 1) * sizeof(char));
    }

    cnst.arr = (char**)malloc(cnst_size * sizeof(char*));
    patterns.arr = (char**)malloc(cnst_size * sizeof(char*));
    for (int i = 0; i < cnst_size; i++)
    {
        cnst.arr[i] = (char*)malloc((len + 1) * sizeof(char));
        patterns.arr[i] = (char*)malloc((len + 1) * sizeof(char));
    }

    ptr = fgets(str, 100, stdin);
    str[14] = 0;
    if (ptr == NULL)
        return 1;
    while (strcmp(str, "+nuova_partita") != 0)
    {
        if(str[0] != '+')
            strcpy(list.arr[list.size++], str);
        ptr = fgets(str, 100, stdin);
        str[14] = 0;
    }


    while (!feof(stdin))
    {
        bool won = false;
        while (strcmp(str, "+nuova_partita") != 0)
        {
            ptr = fgets(str, 100, stdin);
            str[14] = 0;

            if ((strcmp(str, "+inserisci_ini") == 0))
            {
                ptr = fgets(str, 100, stdin);
                str[14] = 0;
                while ((strcmp(str, "+inserisci_fin") != 0))
                {
                    strcpy(list.arr[list.size++], str);

                    ptr = fgets(str, 100, stdin);
                    str[14] = 0;
                }
            }
        }

        filtered.size = list.size;
        for (int i = 0; i < list.size; i++)
        {
            strcpy(filtered.arr[i], list.arr[i]);
        }
        cnst.size = patterns.size = 0;

        ptr = fgets(ref, 100, stdin);
        ptr = fgets(str, 100, stdin);
        count = atoi(str);

        for (i = 0; i < count;)
        {

            ptr = fgets(str, 100, stdin);
            str[14] = 0;

            if ((strcmp(str, "+stampa_filtra") == 0))
            {
                lexicographical_order(filtered);
            }
            else if ((strcmp(str, "+inserisci_ini") == 0))
            {
                ptr = fgets(str, 100, stdin);
                str[14] = 0;

                while ((strcmp(str, "+inserisci_fin") != 0))
                {
                    strcpy(list.arr[list.size++], str);
                    if (check_constraint(str, ref, cnst, patterns))
                        strcpy(filtered.arr[filtered.size++], str);

                    ptr = fgets(str, 100, stdin);
                    str[14] = 0;
                }
            }
            else if ((strcmp(str, "+nuova_partita") == 0))
            {
                break;
            }
            else if (!find_(list, str))
            {
                printf("not_exists\n");
            }
            else
            {
                i++;

                if ((strcmp(str, ref) == 0))
                {
                    printf("ok\n");
                    won = true;
                    break;
                }
                else
                {
                    strcpy(cnst.arr[cnst.size++], str);
                    char pattern[50];
                    generate_code(ref, str, pattern);
                    strcpy(patterns.arr[patterns.size++], pattern);
                    temp = get_filter(&filtered, pattern, str, ref);
                    if(temp == 31)
                        temp = 31;
                    printf("%s\n%d\n", pattern, temp);
                }
            }
        }
        if (i == count && !won)
            printf("ko\n");

        while (true)
        {
            ch = fgetc(stdin);
            if (ch != '\n' && ch != ' ' && ch != '\t')
                break;
        }
        ungetc(ch, stdin);
    }

    for (int i = 0; i < cnst_size; i++)
    {
        free(patterns.arr[i]);
        free(cnst.arr[i]);
    }
    free(cnst.arr);
    free(patterns.arr);

    for (int i = 0; i < max_size; i++)
    {
        free(filtered.arr[i]);
        free(list.arr[i]);
    }
    free(filtered.arr);
    free(list.arr);

    list.arr = patterns.arr = cnst.arr = filtered.arr = NULL;

    return 0;
}
