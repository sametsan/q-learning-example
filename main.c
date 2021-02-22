#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define Y 0.8
#define LR 0.7
#define START 0

struct max_t
{
    int id;
    float value;
};

struct network
{
    int **r;
    float **q;
    int size;
};

void q_add_connect(struct network *net, int s, int a);

void q_add_target(struct network *net, int target);

struct network *q_network_create(int size);

void q_network_free(struct network *net);

int q_random_action(struct network *net, int s);

void q_network_print_r(struct network *net);

void q_network_print_q(struct network *net);

struct max_t max(struct network *net, int a);

int main()
{
    struct network *net = q_network_create(10);

    printf("Adding connection...\n");
    q_add_connect(net, 0, 4);
    q_add_connect(net, 4, 8);
    q_add_connect(net, 8, 3);
    q_add_connect(net, 3, 7);
    q_add_connect(net, 5, 4);
    q_add_connect(net, 0, 5);
    q_add_connect(net, 7, 2);
    q_add_connect(net, 2, 9);
    q_add_target(net, 9);

    printf("Printing network...\n");
    q_network_print_r(net);
    q_network_print_q(net);

    int s = START;
    int exit = 0;

    do
    {

        for (int a = 0; a < net->size; a++)
        {
            if (net->r[s][a] == 100)
                exit = 1;
            if (net->r[s][a] != -1)
                net->q[s][a] = net->q[s][a] + LR * (net->r[s][a] + (Y * max(net, a).value) - net->q[s][a]);

            printf("%d->%d value : %f\n", s, a, net->q[s][a]);
        }

        s = max(net, s).id;

        printf("Status id : %d\n", s);
    } while (!exit);

    q_network_print_q(net);

    q_network_free(net);
}

void q_add_connect(struct network *net, int s, int a)
{
    net->r[s][a] = 0;
    printf("%d->%d connected \n", s, a);
}

void q_add_target(struct network *net, int target)
{
    for (int s = 0; s < net->size; s++)
    {
        if (net->r[s][target] == 0)
            net->r[s][target] = 100;
    }
    net->r[target][target] = 100;
}

struct network *q_network_create(int size)
{
    struct network *net = (struct network *)malloc(sizeof(struct network));
    net->size = size;
    net->r = (int **)malloc(sizeof(int *) * net->size);
    net->q = (float **)malloc(sizeof(float *) * net->size);

    for (int s = 0; s < net->size; s++)
    {
        net->r[s] = (int *)malloc(sizeof(int) * net->size);
        net->q[s] = (float *)malloc(sizeof(float) * net->size);

        for (int a = 0; a < net->size; a++)
        {
            net->r[s][a] = -1;
            net->q[s][a] = 0;
        }
    }

    printf("Network created %dx%d\n", net->size, net->size);
    return net;
}

void q_network_free(struct network *net)
{

    for (int s = 0; s < net->size; s++)
    {
        free(net->r[s]);
        free(net->q[s]);
    }

    free(net->r);
    free(net->q);
}

int q_random_action(struct network *net, int s)
{
    int count = 0;
    int list[net->size];

    for (int i = 0; i < net->size; i++)
    {
        if (net->r[s][i] >= 0)
        {
            list[count] = i;
            count++;
        }
    }

    assert(count != 0);

    srand(time(NULL));
    int r = rand() % count;
    return list[r];
}

void q_network_print_r(struct network *net)
{
    for (int s = 0; s < net->size; s++)
    {
        for (int a = 0; a < net->size; a++)
        {
            printf("%d ", net->r[s][a]);
        }
        printf("\n");
    }
}

void q_network_print_q(struct network *net)
{
    for (int s = 0; s < net->size; s++)
    {
        for (int a = 0; a < net->size; a++)
        {
            printf("%f ", net->q[s][a]);
        }
        printf("\n");
    }
}

struct max_t max(struct network *net, int a)
{
    float max_value = 0;
    struct max_t st = {.id = -1, .value = max_value};

    for (int i = 0; i < net->size; i++)
    {
        if (net->r[a][i] != -1 && net->q[a][i] > max_value)
        {
            st.value = net->q[a][i];
            st.id = i;
            max_value = st.value;
        }
    }

    if (st.id == -1)
    {
        st.id = q_random_action(net, a);
    }

    printf("%d -> %d max value : %f \n", a, st.id, st.value);
    return st;
}