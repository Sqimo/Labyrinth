#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//размеры, желательно mod 5 = 0
#define N 15
#define M 10

//клетка
struct cell_{
    int wall; // стена; 0 - нет, 1 - есть, 2 - выход

    int tre; // настоящий клад; 0 - нет, 1 - есть
    int faketre; // fake клад; 0 - нет, 1 - есть

    int arsenal; // 0 - нет, 1 - есть
    int min; // минотавр; 0 - нет, 1 - живой, 2 - труп
    int river; // река, 0 - нет реки, 1 - наверх, 2 - направо, 3 - вниз, 4 - налево
    int bull; // пули, количество
};

//человек
struct human_ {
    char name[100];
    int x, y; // координаты
    int bull; // пули
    int tre; // клад
    int faketre; // fake клад
};

typedef struct human_ human;
typedef struct cell_ cell;

int main() {

    int P; //количество людей
    int i, j, k, l; //переменные для циклов и тп
    int graveyard[2]; //координаты кладбища
    cell c[N][M];
    human h[100];


    printf("Введите количество игроков\n");
    scanf("%d", &P);


    for(i = 0; i < P; i++) {
        printf("Введите имя игрока номер %d\n", i + 1);
        scanf("\n%s", h[i].name);
        h[i].bull = 3;
        h[i].tre = 0;
        h[i].faketre = 0;
    }


    //зануляем ячейки
    for(i = 0; i < N; i++)
        for(j = 0; j < M; j++) {
            c[i][j].wall = 0;
            c[i][j].river = 0;
            c[i][j].tre = 0;
            c[i][j].faketre = 0;
            c[i][j].arsenal = 0;
            c[i][j].min = 0;
            c[i][j].bull = 0;
        }


    //мировые границы
    for(i = 0; i < N; i++) {
        c[i][0].wall = 1;
        c[i][M - 1].wall = 1;
    }
    for(j = 0; j < M; j++) {
        c[0][j].wall = 1;
        c[N - 1][j].wall = 1;
    }


    srand(time(NULL));
    //создаем выход из лабиринта
    k = rand() % 4;
    i = rand() % (N - 2);
    j = rand() % (M - 2);
    if (k == 0)
        c[i + 1][0].wall = 2;
    if (k == 1)
        c[i + 1][M - 1].wall = 2;
    if (k == 2)
        c[0][j + 1].wall = 2;
    if (k == 3)
        c[N - 1][j + 1].wall = 2;


    //делаем речку
    l = rand() % 2;
    k = rand() % 2;
    int rivx = (rand() % ((N - 2)/2)) + 2 + l * (N - 4)/2;
    int rivy = (rand() % ((M - 2)/2)) + 2 + k * (M - 4)/2 ;
    int rivxf = (rand() % ((N - 2)/2)) + 2  + ((l+1)%2) * (N - 4)/2;
    int rivyf = (rand() % ((M - 2)/2)) + 2  + ((k+1)%2) * (M - 4)/2;
    while((rivx != rivxf) || (rivy != rivyf)) {
        i = rand() % 2;
        if (rivx == rivxf)
            i = 1;
        else if (rivy == rivyf) i = 0;
        if (i == 0) {
            c[rivx][rivy].river = l * 2 + 2;
            if(l == 0)
                rivx += 1;
            else rivx += -1;
        } else {
            c[rivx][rivy].river = ((k + 1) % 2)*2 + 1;
            if(k == 0)
                rivy += 1;
            else rivy += -1;
        }
    }


    //строим стены
    k = 0;
    while(k < (N * M) / 20) {
        l = 0;
        while(l == 0) {
            i = (rand() % (N - 4)) + 2;
            j = (rand() % (M - 4)) + 2;
            if (c[i][j].wall == 0 && c[i][j].river == 0) {
                c[i][j].wall = 1;
                l = 1;
            }
        }
        k++;
    }


    //запускаем минотавров
    k = 0;
    while(k < (N * M) / 50) {
        l = 0;
        while(l == 0) {
            i = rand() % N;
            j = rand() % M;
            if (c[i][j].wall == 0 && c[i][j].river == 0 && c[i][j].min == 0) {
                c[i][j].min = 1;
                l = 1;
            }
        }
        k++;
    }


    //кладем арсенал
    k = 0;
    while(k < (N * M) / 75) {
        l = 0;
        while(l == 0) {
            i = rand() % N;
            j = rand() % M;
            if (c[i][j].wall == 0 && c[i][j].river == 0 && c[i][j].min == 0 && c[i][j].arsenal == 0) {
                c[i][j].arsenal = 1;
                l = 1;
            }
        }
        k++;
    }


    //добавляем клад
    l = 0;
    while(l == 0) {
        i = rand() % N;
        j = rand() % M;
        if (c[i][j].wall == 0 && c[i][j].river == 0 && c[i][j].min == 0 && c[i][j].arsenal == 0) {
            c[i][j].tre = 1;
            l = 1;
        }
    }


    //добавляем ложные клады
    k = 0;
    while(k < (N * M) / 150) {
        l = 0;
        while(l == 0) {
            i = rand() % N;
            j = rand() % M;
            if (c[i][j].wall == 0 && c[i][j].river == 0 && c[i][j].min == 0 && c[i][j].arsenal == 0 && c[i][j].tre == 0 && c[i][j].faketre == 0) {
                c[i][j].faketre = 1;
                l = 1;
            }
        }
        k++;
    }


    //добавляем кладбище
    l = 0;
    while(l == 0) {
        i = rand() % N;
        j = rand() % M;
        if (c[i][j].wall == 0 && c[i][j].river == 0 && c[i][j].min == 0 && c[i][j].arsenal == 0 && c[i][j].tre == 0 && c[i][j].faketre == 0) {
            graveyard[0] = i;
            graveyard[1] = j;
            l = 1;
        }
    }



    /*
    for(j = 0; j < M; j++) {
        for (i = 0; i < N; i++) {
            if (c[i][j].wall == 2) printf("e");
            else if (c[i][j].wall == 1) printf("w");
            else if (c[i][j].river > 0) {if (c[i][j].river == 2 || c[i][j].river == 4) printf("-"); else printf("|"); }
            else if(c[i][j].min == 1) printf("m");
            else if(c[i][j].arsenal == 1) printf("a");
            else if(c[i][j].tre == 1) printf("t");
            else if(c[i][j].faketre == 1) printf("f");
            else if(graveyard[0] == i && graveyard[1] == j) printf("g");
            else printf("0");
        }
        printf("\n");
    }
    */

    printf("Добро пожаловать в наше подземелье! Если хотите прочитать правила введите [y] - да, [n] - нет:\n");
    char f = 'a';
    while (f != 'y' && f != 'n')
        scanf("\n%c", &f);
    if (f == 'y') {
        printf("Правила:\n");
        printf("Вы отважные добровольцы, которые решили завладеть великими сокровищами\n");
        printf("подземелья. Ваша задача - отыскать сокровища и сбежать из подземелья вместе с\n");
        printf("ними. Однако не все так просто, на пути вас будут поджидать стены, реки,\n");
        printf("минотавры, в схватке с которыми вам вряд ли удастся выжить. Однако боги\n");
        printf("подземелья подарили вам бесценный клад - бесмертие. Ни один находящийся в\n");
        printf("подземелье не сможет умереть и будет скитаться до тех пор, пока не сможет\n");
        printf("выйти. Введите любой символ для продолжения...\n");
        scanf("\n%c", &f);
        printf("Основные игровые элементы:\n");
        printf("Игровое поле. Поле 15*10, по которому вы сможете ходить. В клетках может что-то\n");
        printf("быть, а может ничего и не быть.\n");
        printf("Игроки. Игроки перемещаются по полю и вооружены, однако их оружие лишь\n");
        printf("трехзарядное.\n");
        printf("Клад. То, ради чего вы здесь. К сожалению, из-за кромешной тьмы, пока вы не\n");
        printf("найдете выход вы не сможете понять, настоящий ли это клад. А они не все\n");
        printf("настоящие. Му-ха-ха!!!\n");
        printf("Стены. К сожалению, вы не призраки, и сквозь стены ходить не умеете, так что\n");
        printf("придется искать другой путь.\n");
        printf("Выход - дыра во внешней стене. Используя его вы сможете сбежать с кладом. Ну\n");
        printf("или придется вернуться, если вы неаши не тот клад. Или наткнулись на выход с\n");
        printf("пустыми руками.\n");
        printf("Река. Попав в реку, игрок снесется на одну клетку по течению реки, также от нее\n");
        printf("веет прохладой.\n");
        printf("Минотавры. Это почти коты Шредингера, только минотавры. К вашему счастью, они\n");
        printf("прогневали богов, и те решили их не возрождать, так что если вы наткнетесь на\n");
        printf("мертвого минотавра - не бойтесь, он вас не убьет. Наверное. Однако минотавры\n");
        printf("обладают огромной силой, и попав на клетку с живым минотавром вам не жить...\n");
        printf("Кладбище. Умерев, вы потеряете весь свой лут и отправитесь на кладбище, где вас\n");
        printf("возродят и вы сможете продолжить свой путь.\n");
        printf("Арсенал. У вас закончились пули? Не беда. В арсенале пули есть всегда! Введите любой символ для продолжения...\n");
        scanf("\n%c", &f);
        printf("Правила ходов:\nЗа ход игроки походить на соседнюю с ними клетку:\n[w] - вверх\n[s] - вниз\n[d] - направо\n[a] - налево\n");
        printf("Также вы можете выстрелить, если у вас есть пули:\n[w] - вверх\n[s] - вниз\n[d] - направо\n[a] - налево\n");
        printf("Пуля летит, начиная со следующей клетки до тех пор, пока не упрется в стену,\n");
        printf("минотавра или игрока. Последние два погибнут в этом случае. Если в клетке было\n");
        printf("несколько игроков - умрут все. После использования пуля исчезает навсегда.\n");
        printf("В случае смерти, весь лут остается на той клетке, на которой погиб игрок.\n");
        printf("Игроки могут носить сколько угодно кладов, но не более 3 пуль. Если на клетке\n");
        printf("есть лут - вы подбираете все, что можете. Введите любой символ для продолжения...\n");
        scanf("\n%c", &f);
        printf("Удачной игры!\n");
    }

    //размещаю игроков
    k = 0;
    while (k < P) {
        i = rand() % N;
        j = rand() % M;
        if (c[i][j].wall != 0 || c[i][j].tre != 0 || c[i][j].faketre != 0 || c[i][j].min != 0 || c[i][j].river != 0)
            continue;
        h[k].x = i;
        h[k].y = j;
        k++;
    }

    int counter = 1;
    int bulx, buly, check, end_game = 1;
    while(end_game){
        printf("Ход %d\n", counter);
        counter++;
        for(k = 0; k < P; k++) {
            if (c[h[k].x][h[k].y].river != 0) {
                if (c[h[k].x][h[k].y].river == 1)
                    h[k].y -= 1;
                else if (c[h[k].x][h[k].y].river == 2)
                    h[k].x += 1;
                else if (c[h[k].x][h[k].y].river == 3)
                    h[k].y += 1;
                else if (c[h[k].x][h[k].y].river == 4)
                    h[k].x -= 1;
            }
        }
        for(k = 0; k < P; k++) {
            printf("Игрок %s:\n", h[k].name);
            //printf("%d %d\n", h[k].x, h[k].y);
            if (c[h[k].x][h[k].y].min == 1) {
                printf("Рога и копыта победили, минотавр оказался сильней\n");
                c[h[k].x][h[k].y].bull += h[k].bull;
                c[h[k].x][h[k].y].tre += h[k].tre;
                c[h[k].x][h[k].y].faketre += h[k].faketre;
                h[k].bull = 0;
                h[k].faketre = 0;
                h[k].tre = 0;
                h[k].x = graveyard[0];
                h[k].y = graveyard[1];
            }
            if (h[k].x == graveyard[0] && h[k].y == graveyard[1])
                printf("Вас окружают стоны мертвых\n");
            if (c[h[k].x][h[k].y].tre == 1 || c[h[k].x][h[k].y].faketre > 0) {
                if (c[h[k].x][h[k].y].tre + c[h[k].x][h[k].y].faketre == 1)
                    printf("Вы нашли клад!\n");
                else
                    printf("Вы нашли %d реликвий\n", c[h[k].x][h[k].y].tre + c[h[k].x][h[k].y].faketre);
                h[k].tre += c[h[k].x][h[k].y].tre;
                c[h[k].x][h[k].y].tre = 0;
                h[k].faketre += c[h[k].x][h[k].y].faketre;
                c[h[k].x][h[k].y].faketre = 0;
            }
            if (c[h[k].x][h[k].y].bull > 0 && h[k].bull < 3) {
                if (c[h[k].x][h[k].y].bull + h[k].bull > 3) {
                    printf("Вы подобрали %d пуль\n", 3 - h[k].bull);
                    c[h[k].x][h[k].y].bull += h[k].bull - 3;
                    h[k].bull = 3;
                }
                else {
                    printf("Вы подобрали %d пуль\n", c[h[k].x][h[k].y].bull);
                    h[k].bull += c[h[k].x][h[k].y].bull;
                    c[h[k].x][h[k].y].bull = 0;
                }
            }
            if (c[h[k].x][h[k].y].arsenal == 1) {
                h[k].bull = 3;
                printf("Ваш запас пуль восполнен\n");
            }
            if (c[h[k].x][h[k].y].min == 2)
                printf("Вы запнулись о труп минотавра\n");
            if (c[h[k].x + 1][h[k].y].river != 0 || c[h[k].x - 1][h[k].y].river != 0 || c[h[k].x][h[k].y + 1].river != 0 || c[h[k].x][h[k].y - 1].river != 0)
                printf("Вы слышите журчание реки\n");
            if (h[k].bull > 0) {
                printf("Хотите ли вы сделать выстрел? [y] - да, [n] - нет\n");
                f = 'a';
                while (f != 'y' && f != 'n')
                    scanf("\n%c", &f);
                if (f == 'y') {
                    printf("Куда стрелять изволите?\n");
                    bulx = h[k].x;
                    buly = h[k].y;
                    f = 't';
                    while (f != 'w' && f != 's' && f != 'd' && f != 'a')
                        scanf("\n%c", &f);
                    if (f == 'w') {
                        check = 1;
                        while(check) {
                            buly--;
                            if (c[bulx][buly].wall != 0)
                                break;
                            if (c[bulx][buly].min == 1) {
                                c[bulx][buly].min = 2;
                                break;
                            }
                            for (l = 0; l < P; l++) {
                                if (h[l].x == bulx && h[l].y == buly) {
                                    check = 0;
                                    c[h[l].x][h[l].y].bull += h[l].bull;
                                    c[h[l].x][h[l].y].tre += h[l].tre;
                                    c[h[l].x][h[l].y].faketre += h[l].faketre;
                                    h[l].bull = 0;
                                    h[l].faketre = 0;
                                    h[l].tre = 0;
                                    h[l].x = graveyard[0];
                                    h[l].y = graveyard[1];
                                }
                            }
                        }
                        h[k].bull--;
                    }
                    if (f == 'd') {
                        check = 1;
                        while(check) {
                            bulx++;
                            if (c[bulx][buly].wall != 0)
                                break;
                            if (c[bulx][buly].min == 1) {
                                c[bulx][buly].min = 2;
                                break;
                            }
                            for (l = 0; l < P; l++) {
                                if (h[l].x == bulx && h[l].y == buly) {
                                    check = 0;
                                    c[h[l].x][h[l].y].bull += h[l].bull;
                                    c[h[l].x][h[l].y].tre += h[l].tre;
                                    c[h[l].x][h[l].y].faketre += h[l].faketre;
                                    h[l].bull = 0;
                                    h[l].faketre = 0;
                                    h[l].tre = 0;
                                    h[l].x = graveyard[0];
                                    h[l].y = graveyard[1];
                                }
                            }
                        }
                        h[k].bull--;
                    }
                    if (f == 's') {
                        check = 1;
                        while(check) {
                            buly++;
                            if (c[bulx][buly].wall != 0)
                                break;
                            if (c[bulx][buly].min == 1) {
                                c[bulx][buly].min = 2;
                                break;
                            }
                            for (l = 0; l < P; l++) {
                                if (h[l].x == bulx && h[l].y == buly) {
                                    check = 0;
                                    c[h[l].x][h[l].y].bull += h[l].bull;
                                    c[h[l].x][h[l].y].tre += h[l].tre;
                                    c[h[l].x][h[l].y].faketre += h[l].faketre;
                                    h[l].bull = 0;
                                    h[l].faketre = 0;
                                    h[l].tre = 0;
                                    h[l].x = graveyard[0];
                                    h[l].y = graveyard[1];
                                }
                            }
                        }
                        h[k].bull--;
                    }
                    if (f == 'a') {
                        check = 1;
                        while(check) {
                            bulx--;
                            if (c[bulx][buly].wall != 0)
                                break;
                            if (c[bulx][buly].min == 1) {
                                c[bulx][buly].min = 2;
                                break;
                            }
                            for (l = 0; l < P; l++) {
                                if (h[l].x == bulx && h[l].y == buly) {
                                    check = 0;
                                    c[h[l].x][h[l].y].bull += h[l].bull;
                                    c[h[l].x][h[l].y].tre += h[l].tre;
                                    c[h[l].x][h[l].y].faketre += h[l].faketre;
                                    h[l].bull = 0;
                                    h[l].faketre = 0;
                                    h[l].tre = 0;
                                    h[l].x = graveyard[0];
                                    h[l].y = graveyard[1];
                                }
                            }
                        }
                        h[k].bull--;
                    }
                }
            }
            printf("Куда желаете пойти?\n");
            f = 't';
            while (f != 'w' && f != 's' && f != 'd' && f != 'a')
                scanf("\n%c", &f);
            if (f == 'w') {
                if (c[h[k].x][h[k].y - 1].wall == 1) {
                    printf("Вы уперлись в стену. Увы, пройти нельзя\n");
                    continue;
                }
                if (c[h[k].x][h[k].y - 1].wall == 2) {
                    if (h[k].tre == 1) {
                        printf("Игрок %s победил!!!\n", h[k].name);
                        end_game = 0;
                        break;
                    }
                    else if (h[k].faketre > 0) {
                        printf("Ваши старания были напрасны. Это не то, что Вам нужно. Попробуйте ещё\n");
                        h[k].faketre = 0;
                    }
                    else
                        printf("Вы нашли выход, осталось найти клад\n");
                    continue;
                }
                h[k].y--;
                printf("Вы прошли\n");
                if (c[h[k].x][h[k].y].min == 1) {
                    printf("Не стоит вступать в схватку с минотавром...\n");
                    c[h[k].x][h[k].y].bull += h[k].bull;
                    c[h[k].x][h[k].y].tre += h[k].tre;
                    c[h[k].x][h[k].y].faketre += h[k].faketre;
                    h[k].bull = 0;
                    h[k].faketre = 0;
                    h[k].tre = 0;
                    h[k].x = graveyard[0];
                    h[k].y = graveyard[1];
                }
                if (c[h[k].x][h[k].y].river != 0)
                    printf("Вы в реке\n");
            }
            if (f == 'd') {
                if (c[h[k].x + 1][h[k].y].wall == 1) {
                    printf("Вы уперлись в стену. Увы, пройти нельзя\n");
                    continue;
                }
                if (c[h[k].x + 1][h[k].y].wall == 2) {
                    if (h[k].tre == 1) {
                        printf("Игрок %s победил!!!\n", h[k].name);
                        end_game = 0;
                        break;
                    }
                    else if (h[k].faketre > 0) {
                        printf("Ваши старания были напрасны. Это не то, что Вам нужно. Попробуйте ещё\n");
                        h[k].faketre = 0;
                    }
                    else
                        printf("Вы нашли выход, осталось найти клад\n");
                    continue;
                }
                h[k].x++;
                printf("Вы прошли\n");
                if (c[h[k].x][h[k].y].min == 1) {
                    printf("Не стоит вступать в схватку с минотавром...\n");
                    c[h[k].x][h[k].y].bull += h[k].bull;
                    c[h[k].x][h[k].y].tre += h[k].tre;
                    c[h[k].x][h[k].y].faketre += h[k].faketre;
                    h[k].bull = 0;
                    h[k].faketre = 0;
                    h[k].tre = 0;
                    h[k].x = graveyard[0];
                    h[k].y = graveyard[1];
                }
                if (c[h[k].x][h[k].y].river != 0)
                    printf("Вы в реке\n");
            }
            if (f == 's') {
                if (c[h[k].x][h[k].y + 1].wall == 1) {
                    printf("Вы уперлись в стену. Увы, пройти нельзя\n");
                    continue;
                }
                if (c[h[k].x][h[k].y + 1].wall == 2) {
                    if (h[k].tre == 1) {
                        printf("Игрок %s победил!!!\n", h[k].name);
                        end_game = 0;
                        break;
                    }
                    else if (h[k].faketre > 0) {
                        printf("Ваши старания были напрасны. Это не то, что Вам нужно. Попробуйте ещё\n");
                        h[k].faketre = 0;
                    }
                    else
                        printf("Вы нашли выход, осталось найти клад\n");
                    continue;
                }
                h[k].y++;
                printf("Вы прошли\n");
                if (c[h[k].x][h[k].y].min == 1) {
                    printf("Не стоит вступать в схватку с минотавром...\n");
                    c[h[k].x][h[k].y].bull += h[k].bull;
                    c[h[k].x][h[k].y].tre += h[k].tre;
                    c[h[k].x][h[k].y].faketre += h[k].faketre;
                    h[k].bull = 0;
                    h[k].faketre = 0;
                    h[k].tre = 0;
                    h[k].x = graveyard[0];
                    h[k].y = graveyard[1];
                }
                if (c[h[k].x][h[k].y].river != 0)
                    printf("Вы в реке\n");
            }
            if (f == 'a') {
                if (c[h[k].x - 1][h[k].y].wall == 1) {
                    printf("Вы уперлись в стену. Увы, пройти нельзя\n");
                    continue;
                }
                if (c[h[k].x - 1][h[k].y].wall == 2) {
                    if (h[k].tre == 1) {
                        printf("Игрок %s победил!!!\n", h[k].name);
                        end_game = 0;
                        break;
                    }
                    else if (h[k].faketre > 0) {
                        printf("Ваши старания были напрасны. Это не то, что Вам нужно. Попробуйте ещё\n");
                        h[k].faketre = 0;
                    }
                    else
                        printf("Вы нашли выход, осталось найти клад\n");
                    continue;
                }
                h[k].x--;
                printf("Вы прошли\n");
                if (c[h[k].x][h[k].y].min == 1) {
                    printf("Не стоит вступать в схватку с минотавром...\n");
                    c[h[k].x][h[k].y].bull += h[k].bull;
                    c[h[k].x][h[k].y].tre += h[k].tre;
                    c[h[k].x][h[k].y].faketre += h[k].faketre;
                    h[k].bull = 0;
                    h[k].faketre = 0;
                    h[k].tre = 0;
                    h[k].x = graveyard[0];
                    h[k].y = graveyard[1];
                }
                if (c[h[k].x][h[k].y].river != 0)
                    printf("Вы в реке\n");
            }
        }
    }
    return 0;
}
