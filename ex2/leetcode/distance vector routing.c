#include <stdio.h>

#define MAX 10
#define INF 9999

int cost[MAX][MAX];
int dist[MAX];
int nextHop[MAX];
int n;

void printTable(int source)
{
    int i;

    printf("\nDestination\tCost\tNext Hop\n");

    for(i=0;i<n;i++)
    {
        printf("%c\t\t",'A'+i);

        if(dist[i]==INF)
            printf("INF\t-\n");
        else if(i==source)
            printf("0\t-\n");
        else
            printf("%d\t%c\n",dist[i],'A'+nextHop[i]);
    }
}

int main()
{
    int i,j;
    int choice,source;

    printf("Enter Number of Nodes: ");
    scanf("%d",&n);

    printf("Enter Cost Matrix:\n");

    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            scanf("%d",&cost[i][j]);

            if(i!=j && cost[i][j]==0)
                cost[i][j]=INF;
        }
    }

    printf("\nInitial Routing Table (Cost Matrix)\n\n");

    printf("\t");
    for(i=0;i<n;i++)
        printf("%c\t",'A'+i);

    printf("\n");

    for(i=0;i<n;i++)
    {
        printf("%c\t",'A'+i);

        for(j=0;j<n;j++)
        {
            if(cost[i][j]==INF)
                printf("INF\t");
            else
                printf("%d\t",cost[i][j]);
        }

        printf("\n");
    }

    printf("\n1. Routing Table for All Nodes");
    printf("\n2. Routing Table for Particular Node");
    printf("\nEnter Choice: ");
    scanf("%d",&choice);    if(choice==2)
    {
        printf("Enter Source Node (0-%d): ",n-1);
        scanf("%d",&source);

        /* Initialization */
        for(i=0;i<n;i++)
        {
            dist[i]=cost[source][i];

            if(cost[source][i]!=INF)
                nextHop[i]=i;
            else
                nextHop[i]=-1;
        }

        dist[source]=0;

        /* Bellman-Ford */
        for(int k=1;k<n;k++)
        {
            for(i=0;i<n;i++)
            {
                for(j=0;j<n;j++)
                {
                    if(cost[i][j]!=INF && dist[i]!=INF)
                    {
                        if(dist[i]+cost[i][j] < dist[j])
                        {
                            dist[j]=dist[i]+cost[i][j];

                            if(i==source)
                                nextHop[j]=j;
                            else
                                nextHop[j]=nextHop[i];
                        }
                    }
                }
            }
        }

        printf("\nFinal Routing Table for Node %c\n",'A'+source);
        printf("---------------------------------\n");
        printf("Destination\tCost\tNext Hop\n");

        for(i=0;i<n;i++)
        {
            printf("%c\t\t",'A'+i);

            if(dist[i]==INF)
                printf("INF\t-\n");
            else if(i==source)
                printf("0\t-\n");
            else
                printf("%d\t%c\n",dist[i],'A'+nextHop[i]);
        }
    }    else if(choice==1)
    {
        int src,k;

        for(src=0;src<n;src++)
        {
            printf("\n====================================");
            printf("\nRouting Table for Node %c",'A'+src);
            printf("\n====================================\n");

            /* Initialization */
            for(i=0;i<n;i++)
            {
                dist[i]=cost[src][i];

                if(cost[src][i]!=INF)
                    nextHop[i]=i;
                else
                    nextHop[i]=-1;
            }

            dist[src]=0;

            /* Bellman-Ford Algorithm */
            for(k=1;k<n;k++)
            {
                for(i=0;i<n;i++)
                {
                    for(j=0;j<n;j++)
                    {
                        if(cost[i][j]!=INF && dist[i]!=INF)
                        {
                            if(dist[i]+cost[i][j] < dist[j])
                            {
                                dist[j]=dist[i]+cost[i][j];

                                if(i==src)
                                    nextHop[j]=j;
                                else
                                    nextHop[j]=nextHop[i];
                            }
                        }
                    }
                }
            }

            printf("\nDestination\tCost\tNext Hop\n");

            for(i=0;i<n;i++)
            {
                printf("%c\t\t",'A'+i);

                if(dist[i]==INF)
                    printf("INF\t-\n");
                else if(i==src)
                    printf("0\t-\n");
                else
                    printf("%d\t%c\n",dist[i],'A'+nextHop[i]);
            }

            printf("\n");
        }
    }
    else
    {
        printf("Invalid Choice");
    }

    return 0;
}
